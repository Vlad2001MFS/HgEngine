#pragma once
#include "Object.hpp"
#include "WindowEvent.hpp"
#include "hd/Core/FPSCounter.hpp"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace hg {

class RenderDevice;
class RenderSystem2D;
class GUISystem;
class SoundSystem;

struct EngineCreateInfo {
    std::string title = "HgEngine Application";
    glm::ivec2 size = glm::ivec2(640, 480);
    bool isFullscreen = false;

    bool glDebug = true;
    int glMajorVer = 3, glMinorVer = 3;
    bool isCoreProfile = true;
    int depthBits = 16, stencilBits = 8;

    uint32_t freq = 22050;
    uint32_t chunkSize = 4096;
    bool isStereo = true;
};

class BaseApp : public Object {
    HG_OBJECT(BaseApp, Object);
public:
    virtual void onEvent(const WindowEvent &event);
    virtual void onFixedUpdate();
    virtual void onUpdate(float dt);
};

class Engine {
public:
    Engine();

    void initialize(const EngineCreateInfo &createInfo, const hd::StringHash &appHash);
    void shutdown();
    void run();
    void close();

    void setCursorPos(const glm::ivec2 &pos);
    void setCenteredCursorMode(bool mode);
    void setCursorVisible(bool mode);

    bool isKeyDown(KeyCode key) const;
    bool isKeyDown(MouseButton btn) const;
    const EngineCreateInfo &getCreateInfo() const;
    SDL_Window *getWindow() const;
    SDL_GLContext getGLContext() const;
    uint32_t getFps() const;
    float getFrameTime() const;
    BaseApp *getApp();
    glm::ivec2 getWindowSize() const;
    glm::ivec2 getWindowCenter() const;
    float getWindowAspectRatio() const;
    float getDeltaTime() const;
    const hd::Time &getTime() const;
    glm::ivec2 getCursorPos() const;
    const glm::ivec2 &getCursorDelta() const;
    bool isCenteredCursorMode() const;

    RenderDevice &getRenderDevice();
    RenderSystem2D &getRenderSystem2D();
    GUISystem &getGUISystem();
    SoundSystem &getSoundSystem();

private:
    EngineCreateInfo mCreateInfo;
    SDL_Window *mWindow;
    SDL_GLContext mContext;
    hd::FPSCounter mFPSCounter;
    BaseApp *mApp;
    hd::Time mTimer;
    glm::ivec2 mCursorDelta;
    bool mIsCenteredCursorMode;

    RenderDevice *mRenderDevice;
    RenderSystem2D *mRenderSystem2D;
    GUISystem *mGUISystem;
    SoundSystem *mSoundSystem;
};

Engine &getEngine();

}
