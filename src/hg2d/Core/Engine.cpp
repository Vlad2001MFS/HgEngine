#include "Engine.hpp"
#include "hd/Core/hdClock.hpp"

namespace hg2d {

WindowCreateInfo::WindowCreateInfo() : title("HgEngine2D Application") {
    this->width = 640;
    this->height = 480;
    this->fullscreen = false;
}

Engine::Engine(const EngineCreateInfo &createInfo) : mCreateInfo(createInfo), mGameStateSystem(*this) {
    hd::WindowFlags flags = hd::WindowFlags::Resizable;
    if (createInfo.window.fullscreen) {
        flags |= hd::WindowFlags::Fullscreen;
    }
    mWindow.create(createInfo.window.title, createInfo.window.width, createInfo.window.height, flags);
}

Engine::~Engine() {
    mWindow.destroy();
}

void Engine::run() {
    const float UPDATES_COUNT_PER_SEC = 30.0f;
    const hd::Time UPDATE_TIME = hd::Time::fromMilliseconds(1000.0f / UPDATES_COUNT_PER_SEC);

    hd::Time updateTimer;
    bool isExit = false;
    while (!isExit) {
        hd::WindowEvent event;
        while (mWindow.processEvent(event)) {
            if (event.type == hd::WindowEventType::Close) {
                isExit = true;
            }
            mGameStateSystem.onEvent(event);
        }

        if (hd::Clock::getElapsedTime(updateTimer) > UPDATE_TIME) {
            mGameStateSystem.onFixedUpdate();
            updateTimer = hd::Clock::getTime();
        }
        mGameStateSystem.onUpdate();
        mGameStateSystem.onDraw();

        mWindow.swapBuffers();

        mFPSCounter.update();
    }
}

void Engine::close() {
    mWindow.close();
}

bool Engine::isKeyDown(hd::KeyCode key) const {
    return mWindow.isKeyDown(key);
}

bool Engine::isKeyDown(hd::MouseButton button) const {
    return mWindow.isMouseButtonDown(button);
}

const EngineCreateInfo &Engine::getCreateInfo() const {
    return mCreateInfo;
}

uint32_t Engine::getFps() const {
    return mFPSCounter.getFps();
}

float Engine::getFrameTime() const {
    return mFPSCounter.getFrameTime();
}

GameStateSystem &Engine::getGameStateSystem() {
    return mGameStateSystem;
}

}