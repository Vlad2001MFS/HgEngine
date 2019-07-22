#pragma once
#include "../Core/AEngineObject.hpp"
#include "hd/IO/hdImage.hpp"
#include "hd/System/hdWindowEvent.hpp"
#include "3rd/include/glm/glm.hpp"
#include <memory>
#include <string>
#include <vector>

namespace hg2d {

struct Texture;

struct RenderOp {
    RenderOp() : camPos(0, 0, 0), pos(0, 0), size(1, 1), uvOffset(0, 0), uvSize(1, 1) {
        this->texture = nullptr;
        this->angle = 0.0f;
        this->camAngle = 0.0f;
        this->isGUI = false;
    }

    glm::vec3 camPos;
    const Texture *texture;
    glm::vec2 pos, size;
    glm::vec2 uvOffset, uvSize;
    float angle, camAngle;
    bool isGUI;
};

class RenderSystem final : public AEngineObject {
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
    const glm::ivec2 &getTextureSize(const Texture *texture) const;
    const std::string &getTexturePath(const Texture *texture) const;

    void addRenderOp(const RenderOp &rop);

private:
    std::vector<RenderOp> mRenderOps;
    std::vector<RenderOp> mGUIRenderOps;
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}
