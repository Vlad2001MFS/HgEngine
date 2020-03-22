#include "RasterizerState.hpp"

namespace hg {

PolygonOffset::PolygonOffset() {
    this->enabled = false;
    this->factor = 0.0f;
    this->units = 0.0f;
}

PolygonOffset::PolygonOffset(bool enabled, float factor, float units) {
    this->enabled = enabled;
    this->factor = factor;
    this->units = units;
}

RasterizerStateDesc::RasterizerStateDesc() : polygonOffset() {
    this->cullFace = CullFace::None;
    this->fillMode = FillMode::Solid;
    this->frontFace = FrontFace::CCW;
}

RasterizerStateDesc::RasterizerStateDesc(CullFace cullFace, FillMode fillMode, FrontFace frontFace, const PolygonOffset &polygonOffset) : polygonOffset(polygonOffset) {
    this->cullFace = cullFace;
    this->fillMode = fillMode;
    this->frontFace = frontFace;
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