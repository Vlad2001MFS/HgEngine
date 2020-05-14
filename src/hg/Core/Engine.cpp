#include "Engine.hpp"
#include "../Graphics/RenderDevice.hpp"
#include "../Renderer2D/RenderSystem2D.hpp"
#include "../GUI/GUISystem.hpp"
#include "../Sound/SoundSystem.hpp"
#include "hd/Core/Time.hpp"

namespace hg {

static const SDL_Scancode gKeyCodes[] = {
    SDL_SCANCODE_UNKNOWN,

    SDL_SCANCODE_A, SDL_SCANCODE_B, SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E,
    SDL_SCANCODE_F, SDL_SCANCODE_G, SDL_SCANCODE_H, SDL_SCANCODE_I, SDL_SCANCODE_J,
    SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_M, SDL_SCANCODE_N, SDL_SCANCODE_O,
    SDL_SCANCODE_P, SDL_SCANCODE_Q, SDL_SCANCODE_R, SDL_SCANCODE_S, SDL_SCANCODE_T,
    SDL_SCANCODE_U, SDL_SCANCODE_V, SDL_SCANCODE_W, SDL_SCANCODE_X, SDL_SCANCODE_Y,
    SDL_SCANCODE_Z,

    SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
    SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8, SDL_SCANCODE_9,

    SDL_SCANCODE_RETURN, SDL_SCANCODE_ESCAPE, SDL_SCANCODE_BACKSPACE,
    SDL_SCANCODE_TAB, SDL_SCANCODE_SPACE,

    SDL_SCANCODE_F1, SDL_SCANCODE_F2, SDL_SCANCODE_F3, SDL_SCANCODE_F4, SDL_SCANCODE_F5,
    SDL_SCANCODE_F6, SDL_SCANCODE_F7, SDL_SCANCODE_F8, SDL_SCANCODE_F9, SDL_SCANCODE_F10,
    SDL_SCANCODE_F11, SDL_SCANCODE_F12,

    SDL_SCANCODE_PAUSE, SDL_SCANCODE_INSERT, SDL_SCANCODE_HOME, SDL_SCANCODE_PAGEUP, SDL_SCANCODE_PAGEDOWN,
    SDL_SCANCODE_DELETE, SDL_SCANCODE_END, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN,

    SDL_SCANCODE_KP_0, SDL_SCANCODE_KP_1, SDL_SCANCODE_KP_2, SDL_SCANCODE_KP_3, SDL_SCANCODE_KP_4,
    SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_6, SDL_SCANCODE_KP_7, SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_9,

    SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL, SDL_SCANCODE_LSHIFT, SDL_SCANCODE_RSHIFT,
    SDL_SCANCODE_LALT, SDL_SCANCODE_RALT, SDL_SCANCODE_LGUI, SDL_SCANCODE_RGUI,
};

static const uint32_t gMouseButtons[] = {
    SDL_BUTTON_LEFT,
    SDL_BUTTON_RIGHT,
    SDL_BUTTON_MIDDLE,
    SDL_BUTTON_X1,
    SDL_BUTTON_X2
};

KeyCode sdlScancodeToKeyCode(SDL_Scancode key) {
    switch (key) {
        case SDL_SCANCODE_A:
            return KeyCode::A;
        case SDL_SCANCODE_B:
            return KeyCode::B;
        case SDL_SCANCODE_C:
            return KeyCode::C;
        case SDL_SCANCODE_D:
            return KeyCode::D;
        case SDL_SCANCODE_E:
            return KeyCode::E;
        case SDL_SCANCODE_F:
            return KeyCode::F;
        case SDL_SCANCODE_G:
            return KeyCode::G;
        case SDL_SCANCODE_H:
            return KeyCode::H;
        case SDL_SCANCODE_I:
            return KeyCode::I;
        case SDL_SCANCODE_J:
            return KeyCode::J;
        case SDL_SCANCODE_K:
            return KeyCode::K;
        case SDL_SCANCODE_L:
            return KeyCode::L;
        case SDL_SCANCODE_M:
            return KeyCode::M;
        case SDL_SCANCODE_N:
            return KeyCode::N;
        case SDL_SCANCODE_O:
            return KeyCode::O;
        case SDL_SCANCODE_P:
            return KeyCode::P;
        case SDL_SCANCODE_Q:
            return KeyCode::Q;
        case SDL_SCANCODE_R:
            return KeyCode::R;
        case SDL_SCANCODE_S:
            return KeyCode::S;
        case SDL_SCANCODE_T:
            return KeyCode::T;
        case SDL_SCANCODE_U:
            return KeyCode::U;
        case SDL_SCANCODE_V:
            return KeyCode::V;
        case SDL_SCANCODE_W:
            return KeyCode::W;
        case SDL_SCANCODE_X:
            return KeyCode::X;
        case SDL_SCANCODE_Y:
            return KeyCode::Y;
        case SDL_SCANCODE_Z:
            return KeyCode::Z;
        case SDL_SCANCODE_0:
            return KeyCode::Num0;
        case SDL_SCANCODE_1:
            return KeyCode::Num1;
        case SDL_SCANCODE_2:
            return KeyCode::Num2;
        case SDL_SCANCODE_3:
            return KeyCode::Num3;
        case SDL_SCANCODE_4:
            return KeyCode::Num4;
        case SDL_SCANCODE_5:
            return KeyCode::Num5;
        case SDL_SCANCODE_6:
            return KeyCode::Num6;
        case SDL_SCANCODE_7:
            return KeyCode::Num7;
        case SDL_SCANCODE_8:
            return KeyCode::Num8;
        case SDL_SCANCODE_9:
            return KeyCode::Num9;
        case SDL_SCANCODE_RETURN:
            return KeyCode::Return;
        case SDL_SCANCODE_ESCAPE:
            return KeyCode::Escape;
        case SDL_SCANCODE_BACKSPACE:
            return KeyCode::BackSpace;
        case SDL_SCANCODE_TAB:
            return KeyCode::Tab;
        case SDL_SCANCODE_SPACE:
            return KeyCode::Space;
        case SDL_SCANCODE_F1:
            return KeyCode::F1;
        case SDL_SCANCODE_F2:
            return KeyCode::F2;
        case SDL_SCANCODE_F3:
            return KeyCode::F3;
        case SDL_SCANCODE_F4:
            return KeyCode::F4;
        case SDL_SCANCODE_F5:
            return KeyCode::F5;
        case SDL_SCANCODE_F6:
            return KeyCode::F6;
        case SDL_SCANCODE_F7:
            return KeyCode::F7;
        case SDL_SCANCODE_F8:
            return KeyCode::F8;
        case SDL_SCANCODE_F9:
            return KeyCode::F9;
        case SDL_SCANCODE_F10:
            return KeyCode::F10;
        case SDL_SCANCODE_F11:
            return KeyCode::F11;
        case SDL_SCANCODE_F12:
            return KeyCode::F12;
        case SDL_SCANCODE_PAUSE:
            return KeyCode::Pause;
        case SDL_SCANCODE_INSERT:
            return KeyCode::Insert;
        case SDL_SCANCODE_HOME:
            return KeyCode::Home;
        case SDL_SCANCODE_PAGEUP:
            return KeyCode::PageUp;
        case SDL_SCANCODE_PAGEDOWN:
            return KeyCode::PageDown;
        case SDL_SCANCODE_DELETE:
            return KeyCode::Delete;
        case SDL_SCANCODE_END:
            return KeyCode::End;
        case SDL_SCANCODE_LEFT:
            return KeyCode::Left;
        case SDL_SCANCODE_RIGHT:
            return KeyCode::Right;
        case SDL_SCANCODE_UP:
            return KeyCode::Up;
        case SDL_SCANCODE_DOWN:
            return KeyCode::Down;
        case SDL_SCANCODE_KP_0:
            return KeyCode::Numpad0;
        case SDL_SCANCODE_KP_1:
            return KeyCode::Numpad1;
        case SDL_SCANCODE_KP_2:
            return KeyCode::Numpad2;
        case SDL_SCANCODE_KP_3:
            return KeyCode::Numpad3;
        case SDL_SCANCODE_KP_4:
            return KeyCode::Numpad4;
        case SDL_SCANCODE_KP_5:
            return KeyCode::Numpad5;
        case SDL_SCANCODE_KP_6:
            return KeyCode::Numpad6;
        case SDL_SCANCODE_KP_7:
            return KeyCode::Numpad7;
        case SDL_SCANCODE_KP_8:
            return KeyCode::Numpad8;
        case SDL_SCANCODE_KP_9:
            return KeyCode::Numpad9;
        case SDL_SCANCODE_LCTRL:
            return KeyCode::LControl;
        case SDL_SCANCODE_LSHIFT:
            return KeyCode::LShift;
        case SDL_SCANCODE_LALT:
            return KeyCode::LAlt;
        case SDL_SCANCODE_LGUI:
            return KeyCode::LSystem;
        case SDL_SCANCODE_RCTRL:
            return KeyCode::RControl;
        case SDL_SCANCODE_RSHIFT:
            return KeyCode::RShift;
        case SDL_SCANCODE_RALT:
            return KeyCode::RAlt;
        case SDL_SCANCODE_RGUI:
            return KeyCode::RSystem;
        default:
            return KeyCode::Unknown;
    }
}

WindowEvent sdlEventToWindowEvent(const SDL_Event &sdlEvent) {
    WindowEvent e;
    e.sdlEvent = sdlEvent;
    switch (sdlEvent.type) {
        case SDL_QUIT: {
            e.type = WindowEventType::Close;
            break;
        }
        case SDL_WINDOWEVENT: {
            switch (sdlEvent.window.event) {
                case SDL_WINDOWEVENT_RESIZED: {
                    e.type = WindowEventType::Resize;
                    e.resize.w = sdlEvent.window.data1;
                    e.resize.h = sdlEvent.window.data2;
                    break;
                }
                case SDL_WINDOWEVENT_FOCUS_LOST: {
                    e.type = WindowEventType::FocusLost;
                    break;
                }
                case SDL_WINDOWEVENT_FOCUS_GAINED: {
                    e.type = WindowEventType::FocusGained;
                    break;
                }
            }
            break;
        }
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            e.type = WindowEventType::Key;
            if (sdlEvent.type == SDL_KEYDOWN) {
                e.key.state = KeyState::Pressed;
            }
            else {
                e.key.state = KeyState::Released;
            }
            e.key.code = sdlScancodeToKeyCode(sdlEvent.key.keysym.scancode);
            const uint8_t *keys = SDL_GetKeyboardState(nullptr);
            e.key.alt    = keys[SDL_SCANCODE_LALT  ] || keys[SDL_SCANCODE_RALT  ];
            e.key.ctrl   = keys[SDL_SCANCODE_LCTRL ] || keys[SDL_SCANCODE_RCTRL ];
            e.key.shift  = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
            e.key.system = keys[SDL_SCANCODE_LGUI  ] || keys[SDL_SCANCODE_RGUI  ];
            break;
        }
        case SDL_MOUSEWHEEL: {
            e.type = WindowEventType::MouseWheel;
            if (sdlEvent.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                e.mouseWheel.x = -sdlEvent.wheel.x;
                e.mouseWheel.y = -sdlEvent.wheel.y;
            }
            else {
                e.mouseWheel.x = sdlEvent.wheel.x;
                e.mouseWheel.y = sdlEvent.wheel.y;
            }
            uint32_t mouseState = SDL_GetMouseState(nullptr, nullptr);
            e.mouseWheel.leftBtn   = HD_FLAG_EXIST(mouseState, SDL_BUTTON(SDL_BUTTON_LEFT  ));
            e.mouseWheel.middleBtn = HD_FLAG_EXIST(mouseState, SDL_BUTTON(SDL_BUTTON_MIDDLE));
            e.mouseWheel.rightBtn  = HD_FLAG_EXIST(mouseState, SDL_BUTTON(SDL_BUTTON_RIGHT ));
            e.mouseWheel.xBtn1     = HD_FLAG_EXIST(mouseState, SDL_BUTTON(SDL_BUTTON_X1    ));
            e.mouseWheel.xBtn2     = HD_FLAG_EXIST(mouseState, SDL_BUTTON(SDL_BUTTON_X2    ));
            const uint8_t *keys = SDL_GetKeyboardState(nullptr);
            e.mouseWheel.ctrl  = keys[SDL_SCANCODE_LCTRL ] || keys[SDL_SCANCODE_RCTRL ];
            e.mouseWheel.shift = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
            break;
        }
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN: {
            e.type = WindowEventType::MouseButton;
            if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
                e.mouseButton.state = KeyState::Pressed;
            }
            else {
                e.mouseButton.state = KeyState::Released;
            }
            switch (sdlEvent.button.button) {
                case SDL_BUTTON_LEFT: {
                    e.mouseButton.btn = MouseButton::Left;
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    e.mouseButton.btn = MouseButton::Middle;
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    e.mouseButton.btn = MouseButton::Right;
                    break;
                }
                case SDL_BUTTON_X1: {
                    e.mouseButton.btn = MouseButton::X1;
                    break;
                }
                case SDL_BUTTON_X2: {
                    e.mouseButton.btn = MouseButton::X2;
                    break;
                }
            }
            e.mouseButton.x = sdlEvent.button.x;
            e.mouseButton.y = sdlEvent.button.y;
            break;
        }
        case SDL_MOUSEMOTION: {
            e.type = WindowEventType::MouseMove;
            e.mouseMove.x = sdlEvent.motion.x;
            e.mouseMove.y = sdlEvent.motion.y;
            e.mouseMove.deltaX = sdlEvent.motion.xrel;
            e.mouseMove.deltaY = sdlEvent.motion.yrel;
            break;
        }
    }
    return e;
}

void BaseApp::onEvent(const WindowEvent &event) {
}

void BaseApp::onFixedUpdate() {
}

void BaseApp::onUpdate(float dt) {
}

Engine::Engine() : mCursorDelta(0, 0) {
    mWindow = nullptr;
    mContext = nullptr;
    mApp = nullptr;
    mIsCenteredCursorMode = false;
}

void Engine::initialize(const EngineCreateInfo &createInfo, const hd::StringHash &appHash) {
    mTimer = hd::Time::getCurrentTime();
    mCreateInfo = createInfo;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        HD_LOG_ERROR("Failed to initialize SDL2. Error:\n{}", SDL_GetError());
    }

    uint8_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
    if (createInfo.isFullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    if (createInfo.glDebug) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, createInfo.glMajorVer);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, createInfo.glMinorVer);
    if (createInfo.isCoreProfile) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    }
    else {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    }
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, createInfo.depthBits);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, createInfo.stencilBits);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, false);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1);

    int w = createInfo.size.x;
    int h = createInfo.size.y;
    mWindow = SDL_CreateWindow(createInfo.title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
    if (!mWindow) {
        HD_LOG_ERROR("Failed to create window. Error:\n{}", SDL_GetError());
    }

    mContext = SDL_GL_CreateContext(mWindow);
    if (!mContext) {
        HD_LOG_ERROR("Failed to create OpenGL context. Error:\n{}", SDL_GetError());
    }

    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &mCreateInfo.glMajorVer);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &mCreateInfo.glMinorVer);
    if (mCreateInfo.glMajorVer < createInfo.glMajorVer || mCreateInfo.glMinorVer < createInfo.glMinorVer) {
        HD_LOG_ERROR("OpenGL Context version does not match to required. Context {}.{} was created instead {}.{}",
            mCreateInfo.glMajorVer, mCreateInfo.glMinorVer, createInfo.glMajorVer, createInfo.glMinorVer);
    }

    SDL_Event resizeEvent;
    resizeEvent.type = SDL_WINDOWEVENT;
    resizeEvent.window.event = SDL_WINDOWEVENT_RESIZED;
    resizeEvent.window.data1 = createInfo.size.x;
    resizeEvent.window.data2 = createInfo.size.y;
    SDL_PushEvent(&resizeEvent);

    setCenteredCursorMode(false);

    mRenderDevice = new RenderDevice();
    mRenderSystem2D = new RenderSystem2D();
    mGUISystem = new GUISystem();
    mSoundSystem = new SoundSystem();
    mApp = HG_CREATE_OBJECT(appHash, BaseApp);
}

void Engine::shutdown() {
    HD_DELETE(mApp);
    HD_DELETE(mSoundSystem);
    HD_DELETE(mGUISystem);
    HD_DELETE(mRenderSystem2D);
    HD_DELETE(mRenderDevice);
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Engine::run() {
    const float UPDATES_COUNT_PER_SEC = 30.0f;
    const hd::Time UPDATE_TIME = hd::Time::fromMilliseconds(1000.0f / UPDATES_COUNT_PER_SEC);

    hd::Time updateTimer, deltaTimer;
    bool isExit = false;
    while (!isExit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isExit = true;
            }

            WindowEvent e = sdlEventToWindowEvent(event);
            mGUISystem->onEvent(e);
            mApp->onEvent(e);
        }

        if (mIsCenteredCursorMode) {
            SDL_GetRelativeMouseState(&mCursorDelta.x, &mCursorDelta.y);
        }

        if (hd::Time::getElapsedTime(updateTimer) > UPDATE_TIME) {
            mGUISystem->onFixedUpdate();
            mApp->onFixedUpdate();
            updateTimer = hd::Time::getCurrentTime();
        }

        float dt = mFPSCounter.getFrameTime()*0.001f;
        mRenderSystem2D->onUpdate(dt);
        mGUISystem->onUpdate(dt);
        mApp->onUpdate(dt);

        SDL_GL_SwapWindow(mWindow);

        if (mFPSCounter.update()) {
            uint32_t fps = mFPSCounter.getFps();
            float frameTime = mFPSCounter.getFrameTime();
            std::string title = fmt::format("{} | FPS: {:0>4} | FrameTime: {}", mCreateInfo.title, fps, frameTime);
            SDL_SetWindowTitle(mWindow, title.data());
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

bool Engine::isKeyDown(KeyCode code) const {
    return SDL_GetKeyboardState(nullptr)[gKeyCodes[static_cast<size_t>(code)]];
}

bool Engine::isKeyDown(MouseButton button) const {
    uint32_t mouseState = SDL_GetMouseState(nullptr, nullptr);
    return HD_FLAG_EXIST(mouseState, SDL_BUTTON(gMouseButtons[static_cast<size_t>(button)]));
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

BaseApp *Engine::getApp() {
    return mApp;
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

RenderDevice &Engine::getRenderDevice() {
    return *mRenderDevice;
}

RenderSystem2D &Engine::getRenderSystem2D() {
    return *mRenderSystem2D;
}

GUISystem &Engine::getGUISystem() {
    return *mGUISystem;
}

SoundSystem &Engine::getSoundSystem() {
    return *mSoundSystem;
}

Engine &getEngine() {
    static Engine engine;
    return engine;
}

}
