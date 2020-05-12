#pragma once
#include <GL/glew.h>
#include <memory>

namespace hg {

enum class CullFace {
    None,
    Front = GL_FRONT,
    Back = GL_BACK,
    FrontBack = GL_FRONT_AND_BACK
};

enum class FillMode {
    Solid = GL_FILL,
    Wireframe = GL_LINE
};

enum class FrontFace {
    CCW = GL_CCW,
    CW = GL_CW
};

struct PolygonOffset {
    bool enabled = false;
    float factor = 0.0f, units = 0.0f;
};

struct RasterizerStateDesc {
    RasterizerStateDesc &setCullFace(CullFace cullFace);
    RasterizerStateDesc &setFillMode(FillMode fillMode);
    RasterizerStateDesc &setFrontFace(FrontFace frontFace);
    RasterizerStateDesc &setPolygonOffset(bool enabled, float factor, float units);

    CullFace cullFace = CullFace::None;
    FillMode fillMode = FillMode::Solid;
    FrontFace frontFace = FrontFace::CCW;
    PolygonOffset polygonOffset;
};

using RasterizerStatePtr = std::shared_ptr<class RasterizerState>;

class RasterizerState {
public:
    explicit RasterizerState(const RasterizerStateDesc &desc);

    const RasterizerStateDesc &getDesc() const;

    static RasterizerStatePtr create(const RasterizerStateDesc &desc);

private:
    RasterizerStateDesc mDesc;
};

}