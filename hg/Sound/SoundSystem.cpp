#include "SoundSystem.hpp"
#include "../Core/Engine.hpp"
#include "hd/Core/Log.hpp"
#include "hd/IO/FileStream.hpp"
#include "SDL2/SDL_mixer.h"
#include <algorithm>

namespace hg {

struct SoundBuffer {
    std::string name;
    std::vector<uint8_t> buf;
    Mix_Chunk *chunk;
};

struct MusicBuffer {
    std::string name;
    std::vector<uint8_t> buf;
    Mix_Music *music;
};

void SoundSystem::initialize() {
    if (Mix_Init(0) != 0) {
        HD_LOG_ERROR("Failed to initialize SDL_Mixer. Errors: {}", Mix_GetError());
    }
    if (Mix_OpenAudio(static_cast<int>(getEngine().getCreateInfo().freq), AUDIO_S16SYS,
        getEngine().getCreateInfo().isStereo ? 2 : 1, static_cast<int>(getEngine().getCreateInfo().chunkSize)) != 0
        ) {
        HD_LOG_ERROR("Failed to initialize SDL_Mixer audio device. Errors: {}", Mix_GetError());
    }
}

void SoundSystem::shutdown() {
    for (auto &it : mCreatedMusicBuffers) {
        mDestroyMusic(it);
    }
    for (auto &it : mCreatedSoundBuffers) {
        mDestroySound(it);
    }
    Mix_CloseAudio();
    Mix_Quit();
}

SoundBuffer *SoundSystem::createSoundFromFile(const std::string &filename) {
    if (!filename.empty()) {
        SoundBuffer *soundBuffer = new SoundBuffer();
        soundBuffer->name = filename;
        soundBuffer->buf = hd::FileStream(getEngine().getCreateInfo().soundsDataPath + filename, hd::FileMode::Read).readAllBuffer();

        SDL_RWops *rwops = SDL_RWFromConstMem(soundBuffer->buf.data(), static_cast<int>(soundBuffer->buf.size()));
        if (!rwops) {
            HD_LOG_ERROR("Failed to create SDL_RWops from constant memory. Errors: {}", SDL_GetError());
        }
        soundBuffer->chunk = Mix_LoadWAV_RW(rwops, true);
        if (!soundBuffer->chunk) {
            HD_LOG_ERROR("Failed to create sound from file '{}'", filename);
        }

        mCreatedSoundBuffers.push_back(soundBuffer);
        return soundBuffer;
    }
    else {
        HD_LOG_FATAL("Failed to create sound from file. Filename is empty");
        return nullptr;
    }
}

MusicBuffer *SoundSystem::createMusicFromFile(const std::string &filename) {
    if (!filename.empty()) {
        MusicBuffer *musicBuffer = new MusicBuffer();
        musicBuffer->name = filename;
        musicBuffer->buf = hd::FileStream(getEngine().getCreateInfo().soundsDataPath + filename, hd::FileMode::Read).readAllBuffer();

        SDL_RWops *rwops = SDL_RWFromConstMem(musicBuffer->buf.data(), static_cast<int>(musicBuffer->buf.size()));
        if (!rwops) {
            HD_LOG_ERROR("Failed to create SDL_RWops from constant memory. Errors: {}", SDL_GetError());
        }
        musicBuffer->music = Mix_LoadMUS_RW(rwops, true);
        if (!musicBuffer->music) {
            HD_LOG_ERROR("Failed to create music from file '{}'", filename);
        }

        mCreatedMusicBuffers.push_back(musicBuffer);
        return musicBuffer;
    }
    else {
        HD_LOG_FATAL("Failed to create music from file. Filename is empty");
        return nullptr;
    }
}

void SoundSystem::destroySound(SoundBuffer *&soundBuffer) {
    if (!soundBuffer) {
        HD_LOG_WARNING("soundBuffer is nullptr");
    }
    else {
        auto it = std::find(mCreatedSoundBuffers.begin(), mCreatedSoundBuffers.end(), soundBuffer);
        if (it != mCreatedSoundBuffers.end()) {
            mCreatedSoundBuffers.erase(std::remove(mCreatedSoundBuffers.begin(), mCreatedSoundBuffers.end(), soundBuffer), mCreatedSoundBuffers.end());
            mDestroySound(soundBuffer);
        }
        else {
            HD_LOG_WARNING("Failed to destroy sound. The sound wasn't created by SoundSystem");
        }
    }
}

void SoundSystem::destroyMusic(MusicBuffer *&musicBuffer) {
    if (!musicBuffer) {
        HD_LOG_WARNING("musicBuffer is nullptr");
    }
    else {
        auto it = std::find(mCreatedMusicBuffers.begin(), mCreatedMusicBuffers.end(), musicBuffer);
        if (it != mCreatedMusicBuffers.end()) {
            mCreatedMusicBuffers.erase(std::remove(mCreatedMusicBuffers.begin(), mCreatedMusicBuffers.end(), musicBuffer), mCreatedMusicBuffers.end());
            mDestroyMusic(musicBuffer);
        }
        else {
            HD_LOG_WARNING("Failed to destroy music. The music wasn't created by SoundSystem");
        }
    }
}

SoundChannel SoundSystem::playChannel(const SoundBuffer *buffer, int loops) {
    return playChannel(SoundChannel(), buffer, loops);
}

SoundChannel SoundSystem::playChannel(const SoundChannel &channel, const SoundBuffer *buffer, int loops) {
    HD_ASSERT(buffer);
    int playedChannel = Mix_PlayChannel(channel.value, buffer->chunk, loops);
    if (playedChannel == -1) {
        HD_LOG_ERROR("Failed to play sound '{}'. Errors: {}", buffer->name, Mix_GetError());
    }
    return SoundChannel(playedChannel);
}

void SoundSystem::pauseChannel(const SoundChannel &channel) {
    HD_ASSERT(channel);
    Mix_Pause(channel.value);
}

void SoundSystem::resumeChannel(const SoundChannel &channel) {
    HD_ASSERT(channel);
    Mix_Resume(channel.value);
}

void SoundSystem::stopChannel(const SoundChannel &channel) {
    HD_ASSERT(channel);
    Mix_HaltChannel(channel.value);
}

bool SoundSystem::isChannelPlaying(const SoundChannel &channel) const {
    HD_ASSERT(channel);
    return Mix_Playing(channel.value);
}

bool SoundSystem::isChannelPaused(const SoundChannel &channel) const {
    HD_ASSERT(channel);
    return Mix_Paused(channel.value);
}

void SoundSystem::playMusic(const MusicBuffer *buffer, int loops) {
    HD_ASSERT(buffer);
    if (Mix_PlayMusic(buffer->music, loops) != 0) {
        HD_LOG_ERROR("Failed to play music '{}'. Errors: {}", buffer->name, Mix_GetError());
    }
}

void SoundSystem::pauseMusic() {
    Mix_PauseMusic();
}

void SoundSystem::resumeMusic() {
    Mix_ResumeMusic();
}

void SoundSystem::stopMusic() {
    Mix_HaltMusic();
}

bool SoundSystem::isMusicPlaying() const {
    return Mix_PlayingMusic();
}

bool SoundSystem::isMusicPaused() const {
    return Mix_PausedMusic();
}

void SoundSystem::mDestroySound(SoundBuffer*& buffer) {
    if (buffer) {
        mCreatedSoundBuffers.erase(std::remove(mCreatedSoundBuffers.begin(), mCreatedSoundBuffers.end(), buffer), mCreatedSoundBuffers.end());
        Mix_FreeChunk(buffer->chunk);
        HD_DELETE(buffer);
    }
}

void SoundSystem::mDestroyMusic(MusicBuffer*& buffer) {
    if (buffer) {
        mCreatedMusicBuffers.erase(std::remove(mCreatedMusicBuffers.begin(), mCreatedMusicBuffers.end(), buffer), mCreatedMusicBuffers.end());
        Mix_FreeMusic(buffer->music);
        HD_DELETE(buffer);
    }
}

}
