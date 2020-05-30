#pragma once
#include "../Graphics/RenderDevice.hpp"
#include <glm/glm.hpp>

namespace hg {

struct RenderOp {
    Texture2DPtr texture = nullptr;
    glm::vec3 pos = glm::vec3(0, 0, 0);
    glm::vec2 size = glm::vec2(0, 0);
    float angle = 0.0f;
};

class RenderSystem2D {
public:
    RenderSystem2D();
    ~RenderSystem2D();

    void onUpdate(float dt);

    void drawTexture(const Texture2DPtr &texture, const glm::vec3 &pos, const glm::vec2 &size, float angle);
    void drawTextureGUI(const Texture2DPtr &texture, const glm::ivec2 &pos, const glm::ivec2 &size);

    void setCamera(const glm::vec2 &pos, float angle, float distance);

private:
    void mDraw(const glm::mat4 &projView);
    void mDrawGUI(const glm::mat4 &projView);

    glm::vec2 mCamPos = glm::vec2(0, 0);
    float mCamAngle = 0.0f, mCamDistance = 1.0f;
    std::vector<RenderOp> mRenderOps;
    std::vector<RenderOp> mGUIRenderOps;

    VertexFormatPtr mVF;
    BufferPtr mQuadVB, mGUIQuadVB;
    VertexShaderPtr mVS;
    PixelShaderPtr mPS;
    UniformID mQuadVSProjViewMatID, mQuadVSWorldMatID;
    DepthStencilStatePtr mQuadDSS, mGUIQuadDSS;
};

RenderSystem2D &getRenderSystem2D();

}
