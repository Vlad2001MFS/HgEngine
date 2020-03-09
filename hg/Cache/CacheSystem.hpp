#pragma once
#include "../Renderer/RenderDevice.hpp"
#include "hd/Core/Color.hpp"
#include <map>
#include <string>

namespace hg {

struct SoundBuffer;
struct MusicBuffer;

class CacheSystem final : public hd::Singleton<CacheSystem> {
public:
    void initialize();
    void shutdown();

    HTexture2D getTexture(const std::string &filename);
    HTexture2D getTexture(const hd::Color4 &color);
    SoundBuffer *getSound(const std::string &filename);
    MusicBuffer *getMusic(const std::string &filename);

private:
    std::map<std::string, HTexture2D> mTextures;
    std::map<std::string, HTexture2D> mColorTextures;
    std::map<std::string, SoundBuffer*> mSoundBuffers;
    std::map<std::string, MusicBuffer*> mMusicBuffers;
};

inline CacheSystem &getCacheSystem() {
    return CacheSystem::get();
}

}
