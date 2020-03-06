#pragma once
#include "Node.hpp"
#include "hd/Core/FPSCounter.hpp"
#include "SDL2/SDL.h"
#include <memory>

namespace hg {

struct WindowCreateInfo final {
    std::string title = "HgEngine Application";
    glm::ivec2 size = glm::ivec2(640, 480);
    bool fullscreen = false;
};

struct SoundSystemCreateInfo final {
    uint32_t freq = 22050;
    uint32_t chunkSize = 4096;
    bool isStereo = true;
};

struct EngineCreateInfo final {
    WindowCreateInfo window;
    SoundSystemCreateInfo sound;
};

class Engine final : public hd::Singleton<Engine> {
public:
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

    bool isKeyDown(SDL_Scancode key) const;
    const EngineCreateInfo &getCreateInfo() const;
    SDL_Window *getWindow() const;
    SDL_GLContext getGLContext() const;
    uint32_t getFps() const;
    float getFrameTime() const;
    Node *getRoot();
    glm::ivec2 getWindowSize() const;
    glm::ivec2 getWindowCenter() const;

private:
    EngineCreateInfo mCreateInfo;
    SDL_Window *mWindow;
    SDL_GLContext mContext;
    hd::FPSCounter mFPSCounter;
    std::unique_ptr<Node> mRoot;
};

inline Engine &getEngine() {
    return Engine::get();
}

}
