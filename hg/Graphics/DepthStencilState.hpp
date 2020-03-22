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

struct StencilTestDesc {
    StencilTestDesc();
    StencilTestDesc(bool enabled, CompareFunc frontFunc, StencilOp frontFail, StencilOp frontDepthFail, StencilOp frontPass, CompareFunc backFunc, StencilOp backFail, StencilOp backDepthFail, StencilOp backPass, uint32_t readMask = 0xffffffff, uint32_t writeMask = 0xffffffff, int refValue = 0);

    bool enabled;
    CompareFunc frontFunc;
    StencilOp frontFail;
    StencilOp frontDepthFail;
    StencilOp frontPass;
    CompareFunc backFunc;
    StencilOp backFail;
    StencilOp backDepthFail;
    StencilOp backPass;
    uint32_t readMask;
    uint32_t writeMask;
    int refValue;
};

struct DepthTestDesc {
    DepthTestDesc();
    DepthTestDesc(bool enabled, CompareFunc compareFunc, bool writeMask);

    bool enabled;
    CompareFunc compareFunc;
    bool writeMask;
};

using DepthStencilStatePtr = std::shared_ptr<class DepthStencilState>;

class DepthStencilState {
public:
    DepthStencilState(const DepthTestDesc &depthTestDesc, const StencilTestDesc &stencilTestDesc);

    const DepthTestDesc &getDepthTestDesc() const;
    const StencilTestDesc &getStencilTestDesc() const;

    static DepthStencilStatePtr create(const DepthTestDesc &depthTestDesc, const StencilTestDesc &stencilTestDesc);

private:
    DepthTestDesc mDepthTestDesc;
    StencilTestDesc mStencilTestDesc;
};

}