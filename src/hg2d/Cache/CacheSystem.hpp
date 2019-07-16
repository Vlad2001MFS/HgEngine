#pragma once
#include "../Core/AEngineObject.hpp"
#include <map>
#include <string>

namespace hg2d {

struct Texture;
class Font;
struct SoundBuffer;
struct MusicBuffer;

class CacheSystem : public AEngineObject {
public:
    explicit CacheSystem(Engine &engine);

    void onInitialize();
    void onShutdown();

    Texture *loadTexture(const std::string &filename);
    Font *loadFont(const std::string &filename, uint32_t fontSize);
    SoundBuffer *loadSound(const std::string &filename);
    MusicBuffer *loadMusic(const std::string &filename);

private:
    std::map<std::string, Texture*> mTextures;
    std::map<std::string, Font*> mFonts;
    std::map<std::string, SoundBuffer*> mSoundBuffers;
    std::map<std::string, MusicBuffer*> mMusicBuffers;
};

}
