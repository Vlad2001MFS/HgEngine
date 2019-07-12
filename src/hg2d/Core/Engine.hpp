#pragma once
#include "../GameState/GameStateSystem.hpp"
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
    const hd::Window &getWindow() const;
    uint32_t getFps() const;
    float getFrameTime() const;
    GameStateSystem &getGameStateSystem();

private:
    EngineCreateInfo mCreateInfo;
    hd::Window mWindow;
    hd::FPSCounter mFPSCounter;
    GameStateSystem mGameStateSystem;
};

}