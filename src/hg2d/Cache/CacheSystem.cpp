#include "CacheSystem.hpp"
#include "../Core/Engine.hpp"
#include "hd/Core/hdStringUtils.hpp"

namespace hg2d {

CacheSystem::CacheSystem(Engine &engine) : mEngine(engine) {
}

CacheSystem::~CacheSystem() {
}

void CacheSystem::onInitialize() {
}

void CacheSystem::onShutdown() {
    for (auto &texture : mTextures) {
        mEngine.getRenderSystem().destroyTexture(texture.second);
    }
    for (auto &font : mFonts) {
        mEngine.getGUISystem().destroyFont(font.second);
    }
    for (auto &soundBuffer : mSoundBuffers) {
        mEngine.getSoundSystem().destroySound(soundBuffer.second);
    }
    for (auto &musicBuffer : mMusicBuffers) {
        mEngine.getSoundSystem().destroyMusic(musicBuffer.second);
    }
}

Texture *CacheSystem::loadTexture(const std::string &filename) {
    if (!filename.empty()) {
        if (mTextures.count(filename) == 0) {
            Texture *texture = mEngine.getRenderSystem().createTextureFromFile(filename);
            mTextures.insert(std::make_pair(filename, texture));
            return texture;
        }
        else {
            return mTextures.at(filename);
        }
    }
    else {
        HD_LOG_ERROR("Failed to load texture. Filename is empty");
        return nullptr;
    }
}

Font *CacheSystem::loadFont(const std::string &filename, uint32_t fontSize) {
    if (!filename.empty()) {
        std::string name = filename + "@" + std::to_string(fontSize);
        if (mFonts.count(name) == 0) {
            Font *font = mEngine.getGUISystem().createFontFromFile(filename, fontSize);
            mFonts.insert(std::make_pair(name, font));
            return font;
        }
        else {
            return mFonts.at(name);
        }
    }
    else {
        HD_LOG_ERROR("Failed to load font. Filename is empty");
        return nullptr;
    }
}

SoundBuffer *CacheSystem::loadSound(const std::string &filename) {
    if (!filename.empty()) {
        if (mSoundBuffers.count(filename) == 0) {
            SoundBuffer *soundBuffer = mEngine.getSoundSystem().createSoundFromFile(filename);
            mSoundBuffers.insert(std::make_pair(filename, soundBuffer));
            return soundBuffer;
        }
        else {
            return mSoundBuffers.at(filename);
        }
    }
    else {
        HD_LOG_ERROR("Failed to load sound. Filename is empty");
        return nullptr;
    }
}

MusicBuffer *CacheSystem::loadMusic(const std::string &filename) {
    if (!filename.empty()) {
        if (mMusicBuffers.count(filename) == 0) {
            MusicBuffer *musicBuffer = mEngine.getSoundSystem().createMusicFromFile(filename);
            mMusicBuffers.insert(std::make_pair(filename, musicBuffer));
            return musicBuffer;
        }
        else {
            return mMusicBuffers.at(filename);
        }
    }
    else {
        HD_LOG_ERROR("Failed to load music. Filename is empty");
        return nullptr;
    }
}

}

