#pragma once
#include "GLEW/glew.h"
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
    PolygonOffset();
    PolygonOffset(bool enabled, float factor, float units);

    bool enabled;
    float factor, units;
};

struct RasterizerStateDesc {
    RasterizerStateDesc();
    RasterizerStateDesc(CullFace cullFace, FillMode fillMode, FrontFace frontFace, const PolygonOffset &polygonOffset);

    CullFace cullFace;
    FillMode fillMode;
    FrontFace frontFace;
    PolygonOffset polygonOffset;
};

using RasterizerStatePtr = std::shared_ptr<class RasterizerState>;

class RasterizerState {
public:
    RasterizerState(const RasterizerStateDesc &desc);

    const RasterizerStateDesc &getDesc() const;

    static RasterizerStatePtr create(const RasterizerStateDesc &desc);

private:
    RasterizerStateDesc mDesc;
};

}