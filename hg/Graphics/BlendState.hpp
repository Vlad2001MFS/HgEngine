#pragma once
#include "GLEW/glew.h"
#include <memory>

namespace hg {

enum class BlendFactor {
    Zero = GL_ZERO,
    One = GL_ONE,
    SrcColor = GL_SRC_COLOR,
    InvSrcColor = GL_ONE_MINUS_SRC_COLOR,
    SrcAlpha = GL_SRC_ALPHA,
    InvSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
    DestAlpha = GL_DST_ALPHA,
    InvDestAlpha = GL_ONE_MINUS_DST_ALPHA,
    DestColor = GL_DST_COLOR,
    InvDestColor = GL_ONE_MINUS_DST_COLOR,
    SrcAlphaSat = GL_SRC_ALPHA_SATURATE
};

enum class BlendOp {
    Add = GL_FUNC_ADD,
    Subtract = GL_FUNC_SUBTRACT,
    RevSubtract = GL_FUNC_REVERSE_SUBTRACT,
    Min = GL_MIN,
    Max = GL_MAX
};

struct ColorMask {
    ColorMask();
    explicit ColorMask(bool rgba);
    ColorMask(bool r, bool g, bool b, bool a);

    bool r, g, b, a;
};

struct BlendStateDesc {
    BlendStateDesc();
    BlendStateDesc(bool enabled, BlendFactor srcBlend, BlendFactor dstBlend, BlendOp blendOp, BlendFactor srcBlendAlpha, BlendFactor dstBlendAlpha, BlendOp blendOpAlpha, const ColorMask &colorMask);

    bool enabled;
    BlendFactor srcBlend;
    BlendFactor dstBlend;
    BlendOp blendOp;
    BlendFactor srcBlendAlpha;
    BlendFactor dstBlendAlpha;
    BlendOp blendOpAlpha;
    ColorMask colorMask;
};

using BlendStatePtr = std::shared_ptr<class BlendState>;

class BlendState {
public:
    BlendState(const BlendStateDesc &desc);

    const BlendStateDesc &getDesc() const;

    static BlendStatePtr create(const BlendStateDesc &desc);

private:
    BlendStateDesc mDesc;
};

}