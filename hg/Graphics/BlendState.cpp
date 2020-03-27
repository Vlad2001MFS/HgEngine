#include "BlendState.hpp"

namespace hg {

BlendStateDesc &BlendStateDesc::setEnabled(bool enabled) {
    this->enabled = enabled;
    return *this;
}

BlendStateDesc &BlendStateDesc::setSrcFactor(BlendFactor srcFactor) {
    this->srcFactor = srcFactor;
    return *this;
}

BlendStateDesc &BlendStateDesc::setDstFactor(BlendFactor dstFactor) {
    this->dstFactor = dstFactor;
    return *this;
}

BlendStateDesc &BlendStateDesc::setOp(BlendOp op) {
    this->op = op;
    return *this;
}

BlendStateDesc &BlendStateDesc::setSrcAlphaFactor(BlendFactor srcAlphaFactor) {
    this->srcAlphaFactor = srcAlphaFactor;
    return *this;
}

BlendStateDesc &BlendStateDesc::setDstAlphaFactor(BlendFactor dstAlphaFactor) {
    this->dstAlphaFactor = dstAlphaFactor;
    return *this;
}

BlendStateDesc &BlendStateDesc::setOpAlpha(BlendOp opAlpha) {
    this->opAlpha = opAlpha;
    return *this;
}

BlendStateDesc &BlendStateDesc::setColorMask(bool r, bool g, bool b, bool a) {
    this->colorMask.r = r;
    this->colorMask.g = g;
    this->colorMask.b = b;
    this->colorMask.a = a;
    return *this;
}

BlendState::BlendState(const BlendStateDesc &desc) : mDesc(desc) {
}

const BlendStateDesc &BlendState::getDesc() const {
    return mDesc;
}

BlendStatePtr BlendState::create(const BlendStateDesc &desc) {
    return std::make_shared<BlendState>(desc);
}

}