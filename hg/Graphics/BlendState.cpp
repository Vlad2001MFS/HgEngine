#include "BlendState.hpp"

namespace hg {

BlendStateDesc &BlendStateDesc::setEnabled(bool enabled) {
    this->enabled = enabled;
    return *this;
}

BlendStateDesc &BlendStateDesc::setSrcFactor(BlendFactor srcBlend) {
    this->srcBlend = srcBlend;
    return *this;
}

BlendStateDesc &BlendStateDesc::setDstFactor(BlendFactor dstBlend) {
    this->dstBlend = dstBlend;
    return *this;
}

BlendStateDesc &BlendStateDesc::setOp(BlendOp blendOp) {
    this->blendOp = blendOp;
    return *this;
}

BlendStateDesc &BlendStateDesc::setSrcAlphaFactor(BlendFactor srcBlendAlpha) {
    this->srcBlendAlpha = srcBlendAlpha;
    return *this;
}

BlendStateDesc &BlendStateDesc::setDstAlphaFactor(BlendFactor dstBlendAlpha) {
    this->dstBlendAlpha = dstBlendAlpha;
    return *this;
}

BlendStateDesc &BlendStateDesc::setOpAlpha(BlendOp blendOpAlpha) {
    this->blendOpAlpha = blendOpAlpha;
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