#include "BlendState.hpp"

namespace hg {

ColorMask::ColorMask() {
    this->r = true;
    this->g = true;
    this->b = true;
    this->a = true;
}

ColorMask::ColorMask(bool rgba) {
    this->r = rgba;
    this->g = rgba;
    this->b = rgba;
    this->a = rgba;
}

ColorMask::ColorMask(bool r, bool g, bool b, bool a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

BlendStateDesc::BlendStateDesc() : colorMask(true) {
    this->enabled = false;
    this->srcBlend = BlendFactor::One;
    this->dstBlend = BlendFactor::One;
    this->blendOp = BlendOp::Add;
    this->srcBlendAlpha = BlendFactor::One;
    this->dstBlendAlpha = BlendFactor::One;
    this->blendOpAlpha = BlendOp::Add;
}

BlendStateDesc::BlendStateDesc(bool enabled, BlendFactor srcBlend, BlendFactor dstBlend, BlendOp blendOp, BlendFactor srcBlendAlpha, BlendFactor dstBlendAlpha, BlendOp blendOpAlpha, const ColorMask &colorMask) {
    this->enabled = enabled;
    this->srcBlend = srcBlend;
    this->dstBlend = dstBlend;
    this->blendOp = blendOp;
    this->srcBlendAlpha = srcBlendAlpha;
    this->dstBlendAlpha = dstBlendAlpha;
    this->blendOpAlpha = blendOpAlpha;
    this->colorMask = colorMask;
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