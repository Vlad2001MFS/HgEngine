#include "RasterizerState.hpp"

namespace hg {

RasterizerStateDesc &RasterizerStateDesc::setCullFace(CullFace cullFace) {
    this->cullFace = cullFace;
    return *this;
}

RasterizerStateDesc &RasterizerStateDesc::setFillMode(FillMode fillMode) {
    this->fillMode = fillMode;
    return *this;
}

RasterizerStateDesc &RasterizerStateDesc::setFrontFace(FrontFace frontFace) {
    this->frontFace = frontFace;
    return *this;
}

RasterizerStateDesc &RasterizerStateDesc::setPolygonOffset(bool enabled, float factor, float units) {
    this->polygonOffset.enabled = enabled;
    this->polygonOffset.factor = factor;
    this->polygonOffset.units = units;
    return *this;
}

RasterizerState::RasterizerState(const RasterizerStateDesc &desc) : mDesc(desc) {
}

const RasterizerStateDesc &RasterizerState::getDesc() const {
    return mDesc;
}

RasterizerStatePtr RasterizerState::create(const RasterizerStateDesc &desc) {
    return std::make_shared<RasterizerState>(desc);
}

}