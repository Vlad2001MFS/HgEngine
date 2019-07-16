#pragma once
#include "AEngineObject.hpp"
#include "hd/Core/hdFPSCounter.hpp"
#include "hd/System/hdWindow.hpp"
#include <memory>

namespace hg2d {

struct WindowCreateInfo {
    std::string title = "HgEngine2D Application";
    uint32_t width = 640;
    uint32_t height = 480;
    bool fullscreen = false;
};

struct SoundSystemCreateInfo {
    uint32_t freq = 22050;
    uint32_t chunkSize = 4096;
    bool isStereo = true;
};

struct GUISystemCreateInfo {
    std::string fontPath = "font.ttf";
    uint32_t fontSize = 16;
    bool monospacedFont = true;
    hd::Color4 fontColor = hd::Color4::White;
    std::string buttonTexturePath = "button.png";
    std::string hoveredButtonTexturePath = "hoveredButton.png";
    std::string clickedButtonTexturePath = "clickedButton.png";
    int alignSpaceX = 10;
    int alignSpaceY = 10;
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
    SceneSystem &getSceneSystem();

private:
    EngineCreateInfo mCreateInfo;
    hd::Window mWindow;
    hd::FPSCounter mFPSCounter;
    std::unique_ptr<GameStateSystem> mGameStateSystem;
    std::unique_ptr<RenderSystem> mRenderSystem;
    std::unique_ptr<SoundSystem> mSoundSystem;
    std::unique_ptr<GUISystem> mGUISystem;
    std::unique_ptr<CacheSystem> mCacheSystem;
    std::unique_ptr<SceneSystem> mSceneSystem;
};

}