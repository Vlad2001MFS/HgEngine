#pragma once
#include "../Graphics/RenderDevice.hpp"
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

    Texture2DPtr getTexture2D(const std::string &filename);
    Texture2DPtr getTexture2D(const hd::Color4 &color);
    SoundBuffer *getSound(const std::string &filename);
    MusicBuffer *getMusic(const std::string &filename);

private:
    std::map<std::string, Texture2DPtr> mTextures2D;
    std::map<std::string, Texture2DPtr> mColorTextures2D;
    std::map<std::string, SoundBuffer*> mSoundBuffers;
    std::map<std::string, MusicBuffer*> mMusicBuffers;
};

inline CacheSystem &getCacheSystem() {
    return CacheSystem::get();
}

}
