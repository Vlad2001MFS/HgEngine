#include "DepthStencilState.hpp"

namespace hg {

StencilTestDesc::StencilTestDesc() {
    this->enabled = false;
    this->frontFunc = CompareFunc::Always;
    this->frontFail = StencilOp::Keep;
    this->frontDepthFail = StencilOp::Keep;
    this->frontPass = StencilOp::Keep;
    this->backFunc = CompareFunc::Always;
    this->backFail = StencilOp::Keep;
    this->backDepthFail = StencilOp::Keep;
    this->backPass = StencilOp::Keep;
    this->readMask = 0xffffffff;
    this->writeMask = 0xffffffff;
    this->refValue = 0;
}

StencilTestDesc::StencilTestDesc(bool enabled, CompareFunc frontFunc, StencilOp frontFail, StencilOp frontDepthFail, StencilOp frontPass,
    CompareFunc backFunc, StencilOp backFail, StencilOp backDepthFail, StencilOp backPass, uint32_t readMask, uint32_t writeMask, int refValue) {
    this->enabled = enabled;
    this->frontFunc = frontFunc;
    this->frontFail = frontFail;
    this->frontDepthFail = frontDepthFail;
    this->frontPass = frontPass;
    this->backFunc = backFunc;
    this->backFail = backFail;
    this->backDepthFail = backDepthFail;
    this->backPass = backPass;
    this->readMask = readMask;
    this->writeMask = writeMask;
    this->refValue = refValue;
}

DepthTestDesc::DepthTestDesc() {
    this->enabled = false;
    this->compareFunc = CompareFunc::Less;
    this->writeMask = true;
}

DepthTestDesc::DepthTestDesc(bool enabled, CompareFunc compareFunc, bool writeMask) {
    this->enabled = enabled;
    this->compareFunc = compareFunc;
    this->writeMask = writeMask;
}

DepthStencilState::DepthStencilState(const DepthTestDesc &depthTestDesc, const StencilTestDesc &stencilTestDesc)
    : mDepthTestDesc(depthTestDesc), mStencilTestDesc(stencilTestDesc) {
}

const DepthTestDesc &DepthStencilState::getDepthTestDesc() const {
    return mDepthTestDesc;
}

const StencilTestDesc &DepthStencilState::getStencilTestDesc() const {
    return mStencilTestDesc;
}

DepthStencilStatePtr DepthStencilState::create(const DepthTestDesc &depthTestDesc, const StencilTestDesc &stencilTestDesc) {
    return std::make_shared<DepthStencilState>(depthTestDesc, stencilTestDesc);
}

}