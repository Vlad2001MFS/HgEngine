#pragma once
#include "hd/IO/hdImage.hpp"
#include "hd/System/hdWindowEvent.hpp"
#include "3rd/include/glm/glm.hpp"
#include <memory>
#include <string>
#include <vector>

namespace hg2d {

class Engine;
struct Texture;

struct RenderOp {
    RenderOp() : pos(0, 0), size(1, 1), uvOffset(0, 0), uvSize(1, 1) {
        this->texture = nullptr;
        this->angle = 0.0f;
        this->isGUI = false;
    }

    const Texture *texture;
    glm::vec2 pos, size;
    glm::vec2 uvOffset, uvSize;
    float angle;
    bool isGUI;
};

class RenderSystem {
public:
    explicit RenderSystem(Engine &engine);
    ~RenderSystem();

    void onInitialize();
    void onShutdown();
    void onEvent(const hd::WindowEvent &event);
    void onDraw();

    Texture *createTexture(const void *data, uint32_t w, uint32_t h);
    Texture *createTexture(const hd::Image &img);
    Texture *createTextureFromFile(const std::string &path);
    void destroyTexture(Texture *&texture);
    const glm::ivec2 &getTextureSize(Texture *texture) const;

    void addRenderOp(const RenderOp &rop);

private:
    Engine &mEngine;
    std::vector<RenderOp> mRenderOps;
    std::vector<RenderOp> mGUIRenderOps;
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}
