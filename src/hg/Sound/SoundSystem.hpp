#pragma once
#include "hd/Core/Handle.hpp"
#include "hd/Core/StringHash.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace hg {

struct SoundBuffer;
struct MusicBuffer;

using SoundChannel = hd::Handle<int, struct TAG_SoundChannel, -1>;

class SoundSystem {
public:
    SoundSystem();
    ~SoundSystem();

    SoundBuffer *loadSound(const std::string &path);
    MusicBuffer *loadMusic(const std::string &path);

    SoundBuffer *createSoundFromFile(const std::string &path);
    MusicBuffer *createMusicFromFile(const std::string &path);
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
    static std::string mGetFullPath(const std::string &path);

    void mDestroySound(SoundBuffer *&buffer);
    void mDestroyMusic(MusicBuffer *&buffer);

    std::vector<SoundBuffer*> mCreatedSoundBuffers;
    std::vector<MusicBuffer*> mCreatedMusicBuffers;
    std::unordered_map<hd::StringHash, SoundBuffer*> mSoundBuffers;
    std::unordered_map<hd::StringHash, MusicBuffer*> mMusicBuffers;
};

SoundSystem &getSoundSystem();

}
