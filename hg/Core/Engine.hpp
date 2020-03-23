#pragma once
#include "Node.hpp"
#include "hd/Core/FPSCounter.hpp"
#include "SDL2/SDL.h"
#include <memory>

namespace hg {

struct EngineCreateInfo final {
    std::string title = "HgEngine Application";
    glm::ivec2 size = glm::ivec2(640, 480);
    bool fullscreen = false;

    uint32_t freq = 22050;
    uint32_t chunkSize = 4096;
    bool isStereo = true;

    std::string texturesDataPath = "../Data/textures/";
    std::string soundsDataPath = "../Data/sounds/";
};

class Engine final : public hd::Singleton<Engine> {
public:
    Engine();

    void initialize(const EngineCreateInfo &createInfo);
    void shutdown();
    void run();
    void close();

    template<typename T, typename... Args>
    void setRoot(Args &&...args) {
        if (mRoot) {
            HD_LOG_FATAL("Failed to set root node. The node already intialized");
        }
        mRoot = std::make_unique<T>(std::forward(args)...);
    }

    void setCursorPos(const glm::ivec2 &pos);
    void setCenteredCursorMode(bool mode);
    void setCursorVisible(bool mode);

    bool isKeyDown(SDL_Scancode key) const;
    const EngineCreateInfo &getCreateInfo() const;
    SDL_Window *getWindow() const;
    SDL_GLContext getGLContext() const;
    uint32_t getFps() const;
    float getFrameTime() const;
    Node *getRoot();
    glm::ivec2 getWindowSize() const;
    glm::ivec2 getWindowCenter() const;
    float getWindowAspectRatio() const;
    float getDeltaTime() const;
    float getTime() const;
    glm::ivec2 getCursorPos() const;
    const glm::ivec2 &getCursorDelta() const;
    bool isCenteredCursorMode() const;

private:
    EngineCreateInfo mCreateInfo;
    SDL_Window *mWindow;
    SDL_GLContext mContext;
    hd::FPSCounter mFPSCounter;
    std::unique_ptr<Node> mRoot;
    float mDeltaTime;
    hd::Time mTimer;
    glm::ivec2 mCursorDelta;
    bool mIsCenteredCursorMode;
};

inline Engine &getEngine() {
    return Engine::get();
}

}
