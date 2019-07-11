#pragma once
#include "hd/Core/hdFPSCounter.hpp"
#include "hd/System/hdWindow.hpp"

namespace hg2d {

struct WindowCreateInfo {
    std::string title = "HgEngine2D Application";
    uint32_t width = 640;
    uint32_t height = 480;
    bool fullscreen = false;
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
    uint32_t getFps() const;
    float getFrameTime() const;

private:
    EngineCreateInfo mCreateInfo;
    hd::Window mWindow;
    hd::FPSCounter mFPSCounter;
};

}