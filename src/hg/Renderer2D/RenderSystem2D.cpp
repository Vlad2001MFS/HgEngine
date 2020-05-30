#include "RenderSystem2D.hpp"
#include "../Core/Engine.hpp"
#include "hd/Math/MathUtils.hpp"
#include <glm/ext.hpp>

namespace hg {

static const float gQuadVerts[] = {
    -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, // LeftUp
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // LeftDown
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // RightUp

     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // RightUp
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // LeftDown
     0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // RightDown
};

static const float gGUIQuadVerts[] = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // LeftUp
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // LeftDown
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // RightUp

    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // RightUp
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // LeftDown
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // RightDown
};

RenderSystem2D::RenderSystem2D() {
    mVF = VertexFormat::create({
        VertexAttrib(AttribType::Float3, 0, 0, 0, false, false),
        VertexAttrib(AttribType::Float2, 1, 0, sizeof(float[3]), false, false),
    });

    mQuadVB = Buffer::create(gQuadVerts, sizeof(gQuadVerts));
    mGUIQuadVB = Buffer::create(gGUIQuadVerts, sizeof(gGUIQuadVerts));

    mVS = VertexShader::createFromFile("texture.vert");
    mQuadVSProjViewMatID = mVS->getUniformID("gProjViewMat");
    mQuadVSWorldMatID = mVS->getUniformID("gWorldMat");
    mPS = PixelShader::createFromFile("texture.frag");

    mQuadDSS = DepthStencilState::create(DepthStencilTestDesc().
        setDepthEnabled(true)
    );

    mGUIQuadDSS = DepthStencilState::create(DepthStencilTestDesc().
        setDepthEnabled(false)
    );
}

RenderSystem2D::~RenderSystem2D() {
}

void RenderSystem2D::onUpdate(float dt) {
    getRenderDevice().clearRenderTarget(glm::vec4(0.5f, 0.5f, 1.0f, 1.0f));
    getRenderDevice().clearDepthStencil(1.0f, 0);

    getRenderDevice().setBlendState(BlendMode::Alpha);

    glm::vec2 windowSize = getEngine().getWindowSize();

    float aspect = windowSize.y / windowSize.x;
    mProjMat = glm::ortho(-1.0f*mCamDistance, 1.0f*mCamDistance, -1.0f*aspect*mCamDistance, 1.0f*aspect*mCamDistance, -100.0f, 100.0f);
    mViewMat = glm::rotate(glm::mat4(1.0f), -mCamAngle, glm::vec3(0, 0, 1));
    mViewMat = glm::translate(mViewMat, glm::vec3(-mCamPos, 0.0f));
    mInvViewMat = glm::inverse(mViewMat);

    mDraw(mProjMat*mViewMat);
    mRenderOps.clear();

    glm::mat4 projGUI = hd::MathUtils::ortho2D(0, windowSize.x, windowSize.y, 0);
    mDrawGUI(projGUI);
    mGUIRenderOps.clear();
}

void RenderSystem2D::drawTexture(const Texture2DPtr &texture, const glm::vec3 &pos, const glm::vec2 &size, float angle) {
    RenderOp rop;
    rop.texture = texture;
    rop.pos = pos;
    rop.size = size;
    rop.angle = angle;
    mRenderOps.push_back(rop);
}

void RenderSystem2D::drawTextureGUI(const Texture2DPtr &texture, const glm::ivec2 &pos, const glm::ivec2 &size) {
    RenderOp rop;
    rop.texture = texture;
    rop.pos = glm::vec3(pos, 0);
    rop.size = size;
    rop.angle = 0.0f;
    mGUIRenderOps.push_back(rop);
}

void RenderSystem2D::setCamera(const glm::vec2 &pos, float angle, float distance) {
    mCamPos = pos;
    mCamAngle = angle;
    mCamDistance = glm::max(distance, 1.0f);
}

glm::vec2 RenderSystem2D::transformWindowToWorld(const glm::vec2 &pos) const {
    glm::ivec2 wndSize = getEngine().getWindowSize();
    glm::vec2 world = glm::unProject(glm::vec3(pos, 0.0), mInvViewMat, mProjMat, glm::vec4(0, 0, wndSize.x, wndSize.y));
    world.y = -world.y;
    return world;
}

glm::vec2 RenderSystem2D::transformWorldToWindow(const glm::vec2 &pos) const {
    glm::ivec2 wndSize = getEngine().getWindowSize();
    glm::vec2 world = glm::project(glm::vec3(pos, 0.0), mViewMat, mProjMat, glm::vec4(0, 0, wndSize.x, wndSize.y));
    world.y = wndSize.y - world.y;
    return world;
}

void RenderSystem2D::mDraw(const glm::mat4 &projView) {
    getRenderDevice().setDepthStencilState(mQuadDSS);

    getRenderDevice().setVertexShader(mVS);
    getRenderDevice().setPixelShader(mPS);

    mVS->setUniform(mQuadVSProjViewMatID, projView);

    getRenderDevice().setVertexFormat(mVF);
    getRenderDevice().setVertexBuffer(mQuadVB, 0, 0, sizeof(float[5]));
    for (const auto &rop : mRenderOps) {
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), rop.pos);
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), rop.angle, glm::vec3(0, 0, 1));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(rop.size, 1.0f));
        glm::mat4 world = translate*rotate*scale;
        mVS->setUniform(mQuadVSWorldMatID, world);

        getRenderDevice().setTexture(rop.texture, 0);
        getRenderDevice().draw(PrimitiveType::Triangles, 0, 6);
    }
}

void RenderSystem2D::mDrawGUI(const glm::mat4 &projView) {
    getRenderDevice().setDepthStencilState(mGUIQuadDSS);

    getRenderDevice().setVertexShader(mVS);
    getRenderDevice().setPixelShader(mPS);

    mVS->setUniform(mQuadVSProjViewMatID, projView);

    getRenderDevice().setVertexFormat(mVF);
    getRenderDevice().setVertexBuffer(mGUIQuadVB, 0, 0, sizeof(float[5]));
    for (const auto &rop : mGUIRenderOps) {
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), rop.pos);
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), rop.angle, glm::vec3(0, 0, 1));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(rop.size, 1.0f));
        glm::mat4 world = translate*rotate*scale;
        mVS->setUniform(mQuadVSWorldMatID, world);

        getRenderDevice().setTexture(rop.texture, 0);
        getRenderDevice().draw(PrimitiveType::Triangles, 0, 6);
    }
}

RenderSystem2D &getRenderSystem2D() {
    return getEngine().getRenderSystem2D();
}

}
