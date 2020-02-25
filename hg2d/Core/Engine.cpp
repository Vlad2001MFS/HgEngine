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

Engine::Engine(const EngineCreateInfo &createInfo) : mCreateInfo(createInfo) {
    mGameStateSystem = std::make_unique<GameStateSystem>(*this);
    mRenderSystem = std::make_unique<RenderSystem>(*this);
    mSoundSystem = std::make_unique<SoundSystem>(*this);
    mGUISystem = std::make_unique<GUISystem>(*this);
    mCacheSystem = std::make_unique<CacheSystem>(*this);
    mSceneSystem = std::make_unique<SceneSystem>(*this);

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
#   pragma error("Cannot determine RenderSystem to use")
#endif

    mGameStateSystem->onInitialize();
    mRenderSystem->onInitialize();
    mSoundSystem->onInitialize();
    mGUISystem->onInitialize();
    mCacheSystem->onInitialize();
    mSceneSystem->onInitialize();
}

Engine::~Engine() {
    mSceneSystem->onShutdown();
    mCacheSystem->onShutdown();
    mGUISystem->onShutdown();
    mSoundSystem->onShutdown();
    mRenderSystem->onShutdown();
    mGameStateSystem->onShutdown();
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
            mGameStateSystem->onEvent(event);
            mRenderSystem->onEvent(event);
            mGUISystem->onEvent(event);
            mSceneSystem->onEvent(event);
        }

        mGameStateSystem->onUpdate();
        mGUISystem->onUpdate();
        mSceneSystem->onUpdate();
        if (hd::Time::getElapsedTime(updateTimer) > UPDATE_TIME) {
            mGameStateSystem->onFixedUpdate();
            mGUISystem->onFixedUpdate();
            mSceneSystem->onFixedUpdate();
            updateTimer = hd::Time::getCurrentTime();
        }

        mGameStateSystem->onDraw();
        mGUISystem->onDraw();
        mSceneSystem->onDraw();
        mRenderSystem->onDraw();

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

GameStateSystem &Engine::getGameStateSystem() {
    return *mGameStateSystem;
}

RenderSystem &Engine::getRenderSystem() {
    return *mRenderSystem;
}

SoundSystem &Engine::getSoundSystem() {
    return *mSoundSystem;
}

GUISystem &Engine::getGUISystem() {
    return *mGUISystem;
}

CacheSystem &Engine::getCacheSystem() {
    return *mCacheSystem;
}

SceneSystem &Engine::getSceneSystem() {
    return *mSceneSystem;
}

}