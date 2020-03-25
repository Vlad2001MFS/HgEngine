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
    bool r = true;
    bool g = true;
    bool b = true;
    bool a = true;
};

struct BlendStateDesc {
    BlendStateDesc &setEnabled(bool enabled);
    BlendStateDesc &setSrcFactor(BlendFactor srcBlend);
    BlendStateDesc &setDstFactor(BlendFactor dstBlend);
    BlendStateDesc &setOp(BlendOp blendOp);
    BlendStateDesc &setSrcAlphaFactor(BlendFactor srcBlendAlpha);
    BlendStateDesc &setDstAlphaFactor(BlendFactor dstBlendAlpha);
    BlendStateDesc &setOpAlpha(BlendOp blendOpAlpha);
    BlendStateDesc &setColorMask(bool r, bool g, bool b, bool a);

    bool enabled = false;
    BlendFactor srcBlend = BlendFactor::One;
    BlendFactor dstBlend = BlendFactor::One;
    BlendOp blendOp = BlendOp::Add;
    BlendFactor srcBlendAlpha = BlendFactor::One;
    BlendFactor dstBlendAlpha = BlendFactor::One;
    BlendOp blendOpAlpha = BlendOp::Add;
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