#include "Engine.hpp"
#include "../Graphics/RenderDevice.hpp"
#include "../Sound/SoundSystem.hpp"
#include "../GUI/GUISystem.hpp"
#include "../Cache/CacheSystem.hpp"
#include "hd/Core/Time.hpp"

namespace hg {

Engine::Engine() : mCursorDelta(0, 0) {
    mWindow = nullptr;
    mContext = nullptr;
    mIsCenteredCursorMode = false;
}

void Engine::initialize(const EngineCreateInfo &createInfo) {
    mTimer = hd::Time::getCurrentTime();
    mCreateInfo = createInfo;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        HD_LOG_ERROR("Failed to initialize SDL2. Error:\n{}", SDL_GetError());
    }

    uint8_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (createInfo.fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, false);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1);
    mWindow = SDL_CreateWindow(createInfo.title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, createInfo.size.x, createInfo.size.y, flags);
    if (!mWindow) {
        HD_LOG_ERROR("Failed to create window. Error:\n{}", SDL_GetError());
    }

    mContext = SDL_GL_CreateContext(mWindow);
    if (!mContext) {
        HD_LOG_ERROR("Failed to create OpenGL context. Error:\n{}", SDL_GetError());
    }

    SDL_Event resizeEvent;
    resizeEvent.type = SDL_WINDOWEVENT;
    resizeEvent.window.event = SDL_WINDOWEVENT_RESIZED;
    resizeEvent.window.data1 = createInfo.size.x;
    resizeEvent.window.data2 = createInfo.size.y;
    SDL_PushEvent(&resizeEvent);

    setCenteredCursorMode(false);

    getRenderDevice().initialize();
    getSoundSystem().initialize();
    getGUISystem().initialize();
    getCacheSystem().initialize();
}

void Engine::shutdown() {
    mRoot.reset();
    getCacheSystem().shutdown();
    getGUISystem().shutdown();
    getSoundSystem().shutdown();
    getRenderDevice().shutdown();
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
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
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isExit = true;
            }

            getGUISystem().onEvent(event);
            mRoot->onEvent(event);
            getRenderDevice().onEvent(event);
        }

        if (mIsCenteredCursorMode) {
            SDL_GetRelativeMouseState(&mCursorDelta.x, &mCursorDelta.y);
        }

        if (hd::Time::getElapsedTime(updateTimer) > UPDATE_TIME) {
            mRoot->onFixedUpdate();
            updateTimer = hd::Time::getCurrentTime();
        }

        mRoot->onUpdate(mFPSCounter.getFrameTime()*0.001f);
        getGUISystem().onUpdate(mFPSCounter.getFrameTime()*0.001f);

        SDL_GL_SwapWindow(mWindow);

        if (mFPSCounter.update()) {
            SDL_SetWindowTitle(mWindow, fmt::format("{} | FPS: {:0>4} | FrameTime: {}", mCreateInfo.title, mFPSCounter.getFps(), mFPSCounter.getFrameTime()).data());
        }
    }
}

void Engine::close() {
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}

void Engine::setCursorPos(const glm::ivec2 &pos) {
    SDL_WarpMouseInWindow(mWindow, pos.x, pos.y);
}

void Engine::setCenteredCursorMode(bool mode) {
    mIsCenteredCursorMode = mode;
    mCursorDelta = glm::ivec2(0, 0);
    SDL_SetRelativeMouseMode(mode ? SDL_TRUE : SDL_FALSE);
}

void Engine::setCursorVisible(bool mode) {
    SDL_ShowCursor(mode ? SDL_ENABLE : SDL_DISABLE);
}

bool Engine::isKeyDown(SDL_Scancode key) const {
    return SDL_GetKeyboardState(nullptr)[key] != 0;
}

const EngineCreateInfo &Engine::getCreateInfo() const {
    return mCreateInfo;
}

SDL_Window *Engine::getWindow() const {
    return mWindow;
}

SDL_GLContext Engine::getGLContext() const {
    return mContext;
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

glm::ivec2 Engine::getWindowSize() const {
    glm::ivec2 v;
    SDL_GetWindowSize(mWindow, &v.x, &v.y);
    return v;
}

glm::ivec2 Engine::getWindowCenter() const {
    return getWindowSize() / 2;
}

float Engine::getWindowAspectRatio() const {
    return static_cast<float>(getWindowSize().x) / getWindowSize().y;
}

float Engine::getDeltaTime() const {
    return mFPSCounter.getFrameTime();
}

const hd::Time &Engine::getTime() const {
    return mTimer;
}

glm::ivec2 Engine::getCursorPos() const {
    glm::ivec2 pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

bool Engine::isCenteredCursorMode() const {
    return mIsCenteredCursorMode;
}

const glm::ivec2 &Engine::getCursorDelta() const {
    return mCursorDelta;
}

}
