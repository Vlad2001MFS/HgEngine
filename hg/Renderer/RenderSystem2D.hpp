#pragma once
#include "hd/IO/Image.hpp"
#include "hd/Core/Color.hpp"
#include "SDL2/SDL_events.h"
#include "glm/glm.hpp"
#include <memory>
#include <string>
#include <vector>

namespace hg {

struct Texture;

struct RenderOp2D {
    RenderOp2D() : camPos(0, 0, 0), pos(0, 0), size(1, 1), uvOffset(0, 0), uvSize(1, 1) {
        this->texture = nullptr;
        this->angle = 0.0f;
        this->camAngle = 0.0f;
    }

    glm::vec3 camPos;
    const Texture *texture;
    glm::vec2 pos, size;
    glm::vec2 uvOffset, uvSize;
    float angle, camAngle;
};

class RenderSystem2D final : public hd::Singleton<RenderSystem2D> {
public:
    RenderSystem2D();
    ~RenderSystem2D();

    void initialize();
    void shutdown();

    void onEvent(const SDL_Event &event);
    void onDraw();

    Texture *createTexture(const void *data, uint32_t w, uint32_t h);
    Texture *createTexture(const hd::Image &img);
    Texture *createTextureFromFile(const std::string &path);
    Texture *createTextureFromColor(const hd::Color4 &color);
    void destroyTexture(Texture *&texture);
    const glm::ivec2 &getTextureSize(const Texture *texture) const;
    const std::string &getTexturePath(const Texture *texture) const;

    void addRenderOp(const RenderOp2D &rop, bool isGUI);

private:
    std::vector<RenderOp2D> mRenderOps;
    std::vector<RenderOp2D> mGUIRenderOps;
    struct Impl;
    Impl *impl;
};

inline RenderSystem2D &getRenderSystem2D() {
    return RenderSystem2D::get();
}

}
