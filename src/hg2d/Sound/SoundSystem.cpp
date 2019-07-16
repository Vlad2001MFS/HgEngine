#include "SoundSystem.hpp"
#include "../Core/Engine.hpp"
#include <algorithm>

namespace hg2d {

struct SoundBuffer {
    hd::HSound handle;
};

struct MusicBuffer {
    hd::HMusic handle;
};

SoundSystem::SoundSystem(Engine &engine) : AEngineObject(engine) {
}

void SoundSystem::onInitialize() {
    mSoundContext.create(mEngine.getCreateInfo().sound.freq, mEngine.getCreateInfo().sound.chunkSize, mEngine.getCreateInfo().sound.isStereo);
}

void SoundSystem::onShutdown() {
    for (auto &it : mCreatedMusicBuffers) {
        mDestroyMusic(it);
    }
    for (auto &it : mCreatedSoundBuffers) {
        mDestroySound(it);
    }
    mSoundContext.destroy();
}

SoundBuffer *SoundSystem::createSoundFromFile(const std::string &filename) {
    if (!filename.empty()) {
        std::string path = "data/sounds/" + filename;
        SoundBuffer *soundBuffer = new SoundBuffer();
        soundBuffer->handle = mSoundContext.createSoundFromFile(path);
        mCreatedSoundBuffers.push_back(soundBuffer);
        return soundBuffer;
    }
    else {
        HD_LOG_ERROR("Failed to create sound from file. Filename is empty");
        return nullptr;
    }
}

MusicBuffer *SoundSystem::createMusicFromFile(const std::string &filename) {
    if (!filename.empty()) {
        std::string path = "data/sounds/" + filename;
        MusicBuffer *musicBuffer = new MusicBuffer();
        musicBuffer->handle = mSoundContext.createMusicFromFile(path);
        mCreatedMusicBuffers.push_back(musicBuffer);
        return musicBuffer;
    }
    else {
        HD_LOG_ERROR("Failed to create music from file. Filename is empty");
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
    return mSoundContext.playSound(channel, buffer->handle, loops);
}

void SoundSystem::pauseChannel(const SoundChannel &channel) {
	mSoundContext.pauseSound(channel);
}

void SoundSystem::resumeChannel(const SoundChannel &channel) {
	mSoundContext.resumeSound(channel);	
}

void SoundSystem::stopChannel(const SoundChannel &channel) {
	mSoundContext.stopSound(channel);
}

bool SoundSystem::isChannelPlaying(const SoundChannel &channel) const {
	return mSoundContext.isPlayingSound(channel);
}

bool SoundSystem::isChannelPaused(const SoundChannel &channel) const {
    return mSoundContext.isPausedSound(channel);
}

void SoundSystem::playMusic(const MusicBuffer *buffer, int loops) {
	mSoundContext.playMusic(buffer->handle, loops);
}

void SoundSystem::pauseMusic() {
	mSoundContext.pauseMusic();
}

void SoundSystem::resumeMusic() {
	mSoundContext.resumeMusic();
}

void SoundSystem::stopMusic() {
	mSoundContext.stopMusic();
}

bool SoundSystem::isMusicPlaying() const {
	return mSoundContext.isPlayingMusic();
}

bool SoundSystem::isMusicPaused() const {
	return mSoundContext.isPausedMusic();
}

void SoundSystem::mDestroySound(SoundBuffer*& buffer) {
    mSoundContext.destroySound(buffer->handle);
    HD_DELETE(buffer);
}

void SoundSystem::mDestroyMusic(MusicBuffer*& buffer) {
    mSoundContext.destroyMusic(buffer->handle);
    HD_DELETE(buffer);
}

}
