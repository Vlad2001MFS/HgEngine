#include "DepthStencilState.hpp"

namespace hg {

DepthStencilTestDesc &DepthStencilTestDesc::setDepthEnabled(bool enabled) {
    this->depth.enabled = enabled;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setDepthCompareFunc(CompareFunc compareFunc) {
    this->depth.compareFunc = compareFunc;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setDepthWriteMask(bool writeMask) {
    this->depth.writeMask = writeMask;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilEnabled(bool enabled) {
    this->stencil.enabled = enabled;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilFrontCompareFunc(CompareFunc frontFunc) {
    this->stencil.frontFunc = frontFunc;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilFrontFailOp(StencilOp frontFail) {
    this->stencil.frontFail = frontFail;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilFrontDepthFailOp(StencilOp frontDepthFail) {
    this->stencil.frontDepthFail = frontDepthFail;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilFrontPassOp(StencilOp frontPass) {
    this->stencil.frontPass = frontPass;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilBackCompareFunc(CompareFunc backFunc) {
    this->stencil.backFunc = backFunc;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilBackFailOp(StencilOp backFail) {
    this->stencil.backFail = backFail;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilBackDepthFailOp(StencilOp backDepthFail) {
    this->stencil.backDepthFail = backDepthFail;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilBackPassOp(StencilOp backPass) {
    this->stencil.backPass = backPass;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilReadMask(uint32_t readMask) {
    this->stencil.readMask = readMask;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilWriteMask(uint32_t writeMask) {
    this->stencil.writeMask = writeMask;
    return *this;
}

DepthStencilTestDesc &DepthStencilTestDesc::setStencilRefValue(int refValue) {
    this->stencil.refValue = refValue;
    return *this;
}

DepthStencilState::DepthStencilState(const DepthStencilTestDesc &depthStencilTestDesc)
    : mDepthStencilTestDesc(depthStencilTestDesc) {
}

const DepthStencilTestDesc &DepthStencilState::getDesc() const {
    return mDepthStencilTestDesc;
}

DepthStencilStatePtr DepthStencilState::create(const DepthStencilTestDesc &depthStencilTestDesc) {
    return std::make_shared<DepthStencilState>(depthStencilTestDesc);
}

}