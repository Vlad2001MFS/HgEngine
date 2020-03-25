#pragma once
#include "GraphicsDefs.hpp"
#include <memory>

namespace hg {

enum class StencilOp {
    Keep,
    Zero,
    Replace,
    IncrSat,
    DecrSat,
    Invert,
    Incr,
    Decr
};

struct DepthStencilTestDesc {
    DepthStencilTestDesc &setDepthEnabled(bool enabled);
    DepthStencilTestDesc &setDepthCompareFunc(CompareFunc compareFunc);
    DepthStencilTestDesc &setDepthWriteMask(bool writeMask);

    DepthStencilTestDesc &setStencilEnabled(bool enabled);
    DepthStencilTestDesc &setStencilFrontCompareFunc(CompareFunc frontFunc);
    DepthStencilTestDesc &setStencilFrontFailOp(StencilOp frontFail);
    DepthStencilTestDesc &setStencilFrontDepthFailOp(StencilOp frontDepthFail);
    DepthStencilTestDesc &setStencilFrontPassOp(StencilOp frontPass);
    DepthStencilTestDesc &setStencilBackCompareFunc(CompareFunc backFunc);
    DepthStencilTestDesc &setStencilBackFailOp(StencilOp backFail);
    DepthStencilTestDesc &setStencilBackDepthFailOp(StencilOp backDepthFail);
    DepthStencilTestDesc &setStencilBackPassOp(StencilOp backPass);
    DepthStencilTestDesc &setStencilReadMask(uint32_t readMask);
    DepthStencilTestDesc &setStencilWriteMask(uint32_t writeMask);
    DepthStencilTestDesc &setStencilRefValue(int refValue);

    struct DepthTestDesc {
        bool enabled = false;
        CompareFunc compareFunc = CompareFunc::Less;
        bool writeMask = true;
    } depth;

    struct StencilTestDesc {
        bool enabled = false;
        CompareFunc frontFunc = CompareFunc::Always;
        StencilOp frontFail = StencilOp::Keep;
        StencilOp frontDepthFail = StencilOp::Keep;
        StencilOp frontPass = StencilOp::Keep;
        CompareFunc backFunc = CompareFunc::Always;
        StencilOp backFail = StencilOp::Keep;
        StencilOp backDepthFail = StencilOp::Keep;
        StencilOp backPass = StencilOp::Keep;
        uint32_t readMask = 0xffffffff;
        uint32_t writeMask = 0xffffffff;
        int refValue = 0;
    } stencil;
};

using DepthStencilStatePtr = std::shared_ptr<class DepthStencilState>;

class DepthStencilState {
public:
    DepthStencilState(const DepthStencilTestDesc &depthStencilTestDesc);

    const DepthStencilTestDesc &getDesc() const;

    static DepthStencilStatePtr create(const DepthStencilTestDesc &depthStencilTestDesc);

private:
    DepthStencilTestDesc mDepthStencilTestDesc;
};

}