#pragma once
#include "hd/Core/Common.hpp"
#include "hd/Core/Color.hpp"
#include <map>
#include <string>

namespace hg {

struct Texture;
struct SoundBuffer;
struct MusicBuffer;

class CacheSystem final : public hd::Singleton<CacheSystem> {
public:
    void initialize();
    void shutdown();

    Texture *loadTexture(const std::string &filename);
    Texture *loadTexture(const hd::Color4 &color);
    SoundBuffer *loadSound(const std::string &filename);
    MusicBuffer *loadMusic(const std::string &filename);

private:
    std::map<std::string, Texture*> mTextures;
    std::map<std::string, Texture*> mColorTextures;
    std::map<std::string, SoundBuffer*> mSoundBuffers;
    std::map<std::string, MusicBuffer*> mMusicBuffers;
};

inline CacheSystem &getCacheSystem() {
    return CacheSystem::get();
}

}
