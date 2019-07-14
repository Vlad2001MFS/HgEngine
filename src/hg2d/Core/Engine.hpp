#pragma once
#include "../GameState/GameStateSystem.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../Sound/SoundSystem.hpp"
#include "../GUI/GUISystem.hpp"
#include "../Cache/CacheSystem.hpp"
#include "hd/Core/hdFPSCounter.hpp"
#include "hd/System/hdWindow.hpp"

namespace hg2d {

struct WindowCreateInfo {
    WindowCreateInfo();

    std::string title;
    uint32_t width;
    uint32_t height;
    bool fullscreen;
};

struct EngineCreateInfo {
    WindowCreateInfo window;
    SoundSystemCreateInfo sound;
    GUISystemCreateInfo gui;
};

class Engine {
public:
    explicit Engine(const EngineCreateInfo &createInfo);
    ~Engine();

    void run();
    void close();

    bool isKeyDown(hd::KeyCode key) const;
    bool isKeyDown(hd::MouseButton button) const;
    const EngineCreateInfo &getCreateInfo() const;
    hd::Window &getWindow();
    uint32_t getFps() const;
    float getFrameTime() const;
    GameStateSystem &getGameStateSystem();
    RenderSystem &getRenderSystem();
    SoundSystem &getSoundSystem();
    GUISystem &getGUISystem();
    CacheSystem &getCacheSystem();

private:
    EngineCreateInfo mCreateInfo;
    hd::Window mWindow;
    hd::FPSCounter mFPSCounter;
    GameStateSystem mGameStateSystem;
    RenderSystem mRenderSystem;
    SoundSystem mSoundSystem;
    GUISystem mGUISystem;
    CacheSystem mCacheSystem;
};

}