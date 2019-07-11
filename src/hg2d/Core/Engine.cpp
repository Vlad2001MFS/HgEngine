#include "Engine.hpp"

namespace hg2d {

Engine::Engine(const EngineCreateInfo &createInfo) {
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
    bool isExit = false;
    while (!isExit) {
        hd::WindowEvent event;
        while (mWindow.processEvent(event)) {
            if (event.type == hd::WindowEventType::Close) {
                isExit = true;
            }
        }

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

}