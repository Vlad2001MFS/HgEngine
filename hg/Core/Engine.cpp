#include "Engine.hpp"
#include "BuildConfig.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../Sound/SoundSystem.hpp"
#include "../GUI/GUISystem.hpp"
#include "../Cache/CacheSystem.hpp"
#include "hd/Core/Time.hpp"

namespace hg {

void Engine::initialize(const EngineCreateInfo &createInfo) {
    mCreateInfo = createInfo;

    hd::WindowFlags flags = hd::WindowFlags::Resizable;
    if (createInfo.window.fullscreen) {
        flags |= hd::WindowFlags::Fullscreen;
    }
#ifdef HG_RENDERER_D3D11
    mWindow.create(createInfo.window.title, createInfo.window.size, flags);
#elif defined(HG_RENDERER_OPENGL4)
#ifdef HD_BUILDMODE_DEBUG
    mWindow.create(createInfo.window.title, createInfo.window.size, flags, hd::OpenGLContextSettings(4, 5, 0, 0, 0, false, true));
#else
    mWindow.create(createInfo.window.title, createInfo.window.size, flags, hd::OpenGLContextSettings(4, 5, 0, 0, 0, false, false));
#endif
#else
#   pragma error("Cannot determine which RenderSystem to use")
#endif

    getRenderSystem().initialize();
    getSoundSystem().initialize();
    getGUISystem().initialize();
    getCacheSystem().initialize();
}

void Engine::shutdown() {
    mRoot.reset();
    getCacheSystem().shutdown();
    getGUISystem().shutdown();
    getSoundSystem().shutdown();
    getRenderSystem().shutdown();
    mWindow.destroy();
}

void Engine::run() {
    const float UPDATES_COUNT_PER_SEC = 30.0f;
    const hd::Time UPDATE_TIME = hd::Time::fromMilliseconds(1000.0f / UPDATES_COUNT_PER_SEC);

    if (!mRoot) {
        HD_LOG_FATAL("The root node must be initialized");
    }

    hd::Time updateTimer, deltaTimer;
    bool isExit = false;
    while (!isExit) {
        float dt = hd::Time::getElapsedTime(deltaTimer).getMilliseconds();
        deltaTimer = hd::Time::getCurrentTime();

        hd::WindowEvent event;
        while (mWindow.processEvent(event)) {
            if (event.type == hd::WindowEventType::Close) {
                isExit = true;
            }

            getGUISystem().onEvent(event);
            mRoot->onEvent(event);
            getRenderSystem().onEvent(event);
        }

        mRoot->onUpdate(dt);
        if (hd::Time::getElapsedTime(updateTimer) > UPDATE_TIME) {
            mRoot->onFixedUpdate();
            updateTimer = hd::Time::getCurrentTime();
        }

        mRoot->onDraw();
        getRenderSystem().onDraw();

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
    return mWindow.isKeyDown(button);
}

const EngineCreateInfo &Engine::getCreateInfo() const {
    return mCreateInfo;
}

hd::Window &Engine::getWindow() {
    return mWindow;
}

uint32_t Engine::getFps() const {
    return mFPSCounter.getFps();
}

float Engine::getFrameTime() const {
    return mFPSCounter.getFrameTime();
}

Node *Engine::getRoot() {
    return mRoot.get();
}

}
