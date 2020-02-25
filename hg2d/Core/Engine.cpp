#include "Engine.hpp"
#include "BuildConfig.hpp"
#include "../GameState/GameStateSystem.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../Sound/SoundSystem.hpp"
#include "../GUI/GUISystem.hpp"
#include "../Cache/CacheSystem.hpp"
#include "../Scene/SceneSystem.hpp"
#include "hd/Core/Time.hpp"

namespace hg2d {

void Engine::initialize(const EngineCreateInfo &createInfo) {
    mCreateInfo = createInfo;

    hd::WindowFlags flags = hd::WindowFlags::Resizable;
    if (createInfo.window.fullscreen) {
        flags |= hd::WindowFlags::Fullscreen;
    }
#ifdef HG2D_RENDERER_D3D11
    mWindow.create(createInfo.window.title, createInfo.window.size, flags);
#elif defined(HG2D_RENDERER_OPENGL4)
#ifdef HD_BUILDMODE_DEBUG
    mWindow.create(createInfo.window.title, createInfo.window.size, flags, hd::OpenGLContextSettings(4, 5, 0, 0, 0, true, true));
#else
    mWindow.create(createInfo.window.title, createInfo.window.size, flags, hd::OpenGLContextSettings(4, 5, 0, 0, 0, true, false));
#endif
#else
#   pragma error("Cannot determine which RenderSystem to use")
#endif

    getGameStateSystem().initialize();
    getRenderSystem().initialize();
    getSoundSystem().initialize();
    getGUISystem().initialize();
    getCacheSystem().initialize();
    getSceneSystem().initialize();
}

void Engine::shutdown() {
    getSceneSystem().shutdown();
    getCacheSystem().shutdown();
    getGUISystem().shutdown();
    getSoundSystem().shutdown();
    getRenderSystem().shutdown();
    getGameStateSystem().shutdown();
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
            getGameStateSystem().onEvent(event);
            getRenderSystem().onEvent(event);
            getGUISystem().onEvent(event);
            getSceneSystem().onEvent(event);
        }

        getGameStateSystem().onUpdate();
        getGUISystem().onUpdate();
        getSceneSystem().onUpdate();
        if (hd::Time::getElapsedTime(updateTimer) > UPDATE_TIME) {
            getGameStateSystem().onFixedUpdate();
            getGUISystem().onFixedUpdate();
            getSceneSystem().onFixedUpdate();
            updateTimer = hd::Time::getCurrentTime();
        }

        getGameStateSystem().onDraw();
        getGUISystem().onDraw();
        getSceneSystem().onDraw();
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

}