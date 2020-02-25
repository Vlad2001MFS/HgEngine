#pragma once
#include "../Core/AEngineObject.hpp"
#include "hd/Sound/SoundContext.hpp"

namespace hg2d {

struct SoundBuffer;
struct MusicBuffer;

using SoundChannel = hd::HSoundChannel;

class SoundSystem final : public AEngineObject {
public:
    explicit SoundSystem(Engine &engine);

    void onInitialize();
    void onShutdown();

    SoundBuffer *createSoundFromFile(const std::string &filename);
    MusicBuffer *createMusicFromFile(const std::string &filename);
    void destroySound(SoundBuffer *&soundBuffer);
    void destroyMusic(MusicBuffer *&musicBuffer);

    SoundChannel playChannel(const SoundBuffer *buffer, int loops = 0);
    SoundChannel playChannel(const SoundChannel &channel, const SoundBuffer *buffer, int loops = 0);
    void pauseChannel(const SoundChannel &channel);
    void resumeChannel(const SoundChannel &channel);
    void stopChannel(const SoundChannel &channel);
    bool isChannelPlaying(const SoundChannel &channel) const;
    bool isChannelPaused(const SoundChannel &channel) const;

    void playMusic(const MusicBuffer *buffer, int loops);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
    bool isMusicPlaying() const;
    bool isMusicPaused() const;

private:
    void mDestroySound(SoundBuffer *&buffer);
    void mDestroyMusic(MusicBuffer *&buffer);

    hd::SoundContext mSoundContext;
    std::vector<SoundBuffer*> mCreatedSoundBuffers;
    std::vector<MusicBuffer*> mCreatedMusicBuffers;
};

}
