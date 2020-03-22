#include "CacheSystem.hpp"
#include "../Sound/SoundSystem.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/Core/Log.hpp"

namespace hg {

void CacheSystem::initialize() {
}

void CacheSystem::shutdown() {
    for (auto &soundBuffer : mSoundBuffers) {
        getSoundSystem().destroySound(soundBuffer.second);
    }
    for (auto &musicBuffer : mMusicBuffers) {
        getSoundSystem().destroyMusic(musicBuffer.second);
    }
}

Texture2DPtr CacheSystem::getTexture2D(const std::string &filename) {
    if (!filename.empty()) {
        if (mTextures2D.count(filename) == 0) {
            Texture2DPtr texture = Texture2D::createFromFile(filename);
            mTextures2D.insert(std::make_pair(filename, texture));
            return texture;
        }
        else {
            return mTextures2D.at(filename);
        }
    }
    else {
        HD_LOG_FATAL("Failed to load texture. Filename is empty");
        return Texture2DPtr();
    }
}

Texture2DPtr CacheSystem::getTexture2D(const hd::Color4 &color) {
    std::string key = fmt::format("{} {} {} {}", color.r, color.g, color.b, color.a);
    if (mColorTextures2D.count(key) == 0) {
        Texture2DPtr texture = Texture2D::create(&color, glm::ivec2(1, 1), hg::TextureFormat::RGBA8);
        mColorTextures2D.insert(std::make_pair(key, texture));
        return texture;
    }
    else {
        return mColorTextures2D.at(key);
    }
}

SoundBuffer *CacheSystem::getSound(const std::string &filename) {
    if (!filename.empty()) {
        if (mSoundBuffers.count(filename) == 0) {
            SoundBuffer *soundBuffer = getSoundSystem().createSoundFromFile(filename);
            mSoundBuffers.insert(std::make_pair(filename, soundBuffer));
            return soundBuffer;
        }
        else {
            return mSoundBuffers.at(filename);
        }
    }
    else {
        HD_LOG_FATAL("Failed to load sound. Filename is empty");
        return nullptr;
    }
}

MusicBuffer *CacheSystem::getMusic(const std::string &filename) {
    if (!filename.empty()) {
        if (mMusicBuffers.count(filename) == 0) {
            MusicBuffer *musicBuffer = getSoundSystem().createMusicFromFile(filename);
            mMusicBuffers.insert(std::make_pair(filename, musicBuffer));
            return musicBuffer;
        }
        else {
            return mMusicBuffers.at(filename);
        }
    }
    else {
        HD_LOG_FATAL("Failed to load music. Filename is empty");
        return nullptr;
    }
}

}

