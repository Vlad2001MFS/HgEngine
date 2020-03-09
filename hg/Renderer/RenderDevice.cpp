#include "RenderDevice.hpp"
#include "../Core/Engine.hpp"
#include "hd/IO/FileStream.hpp"
#include "hd/IO/Image.hpp"
#include "hd/Math/MathUtils.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/Core/Log.hpp"
#include "GLEW/glew.h"

namespace hg {

static const uint32_t MAX_VERTEX_BUFFERS = 8;
static const uint32_t MAX_CONSTANT_BUFFERS = 8;
static const uint32_t MAX_TEXTURES = 8;
static const uint32_t MAX_SAMPLER_STATES = MAX_TEXTURES;

static const GLenum gVertexElementSizes[] = {
    1,
    2,
    3,
    4,
};

static const GLenum gVertexElementTypes[] = {
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT
};

static const GLenum gBufferUsages[] = {
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW,
    GL_STREAM_DRAW
};

static const GLenum gBufferAccesses[] = {
    GL_READ_ONLY,
    GL_WRITE_ONLY,
    GL_READ_WRITE
};

static const int gTextureInternalFormats[] = {
    GL_R8,
    GL_RG8,
    GL_RGB8,
    GL_RGBA8,
    GL_ALPHA,
    GL_R16,
    GL_RG16,
    GL_RGB16,
    GL_RGBA16,
    GL_R16F,
    GL_RG16F,
    GL_RGB16F,
    GL_RGBA16F,
    GL_R32F,
    GL_RG32F,
    GL_RGB32F,
    GL_RGBA32F,
    GL_DEPTH_COMPONENT16,
    GL_DEPTH_COMPONENT24,
    GL_DEPTH_COMPONENT32,
    GL_DEPTH24_STENCIL8,
};
static const GLenum gTextureExternalFormats[] = {
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_ALPHA,
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_DEPTH_COMPONENT,
    GL_DEPTH_COMPONENT,
    GL_DEPTH_COMPONENT,
    GL_DEPTH_STENCIL,
};
static const GLenum gTextureDataTypes[] = {
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT,
    GL_UNSIGNED_INT,
    GL_UNSIGNED_INT_24_8,
};

static const GLint gSamplerAddressModes[] = {
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_BORDER,
    GL_MIRROR_CLAMP_EXT
};

constexpr GLenum gPrimitiveTypes[] = {
    GL_POINTS,
    GL_LINE_STRIP,
    GL_LINES,
    GL_TRIANGLE_STRIP,
    GL_TRIANGLES,
};

constexpr GLenum gCompareFuncs[] = {
    GL_NEVER,
    GL_LESS,
    GL_EQUAL,
    GL_LEQUAL,
    GL_GREATER,
    GL_NOTEQUAL,
    GL_GEQUAL,
    GL_ALWAYS
};

constexpr GLenum gStencilOps[] =
{
    GL_KEEP,
    GL_ZERO,
    GL_REPLACE,
    GL_INCR,
    GL_DECR,
    GL_INVERT,
    GL_INCR_WRAP,
    GL_DECR_WRAP,
};

constexpr GLenum gCullFaces[] = {
    0,
    GL_FRONT,
    GL_BACK,
    GL_FRONT_AND_BACK
};

constexpr GLenum gFillModes[] = {
    GL_FILL,
    GL_LINE
};

constexpr GLenum gFrontFaces[] = {
    GL_CCW,
    GL_CW
};

constexpr int gBlendFactors[] = {
    GL_ZERO,
    GL_ONE,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_SRC_ALPHA_SATURATE,
};

constexpr int gBlendOps[] = {
    GL_FUNC_ADD,
    GL_FUNC_SUBTRACT,
    GL_FUNC_REVERSE_SUBTRACT,
    GL_MIN,
    GL_MAX,
};

const BlendModeDesc gBlendModeDescs[] = {
    BlendModeDesc(false, BlendFactor::One         , BlendFactor::Zero       , BlendOp::Add        , BlendFactor::One         , BlendFactor::Zero       , BlendOp::Add),
    BlendModeDesc(true,  BlendFactor::One         , BlendFactor::One        , BlendOp::Add        , BlendFactor::One         , BlendFactor::One        , BlendOp::Add),
    BlendModeDesc(true,  BlendFactor::DestColor   , BlendFactor::Zero       , BlendOp::Add        , BlendFactor::One         , BlendFactor::Zero       , BlendOp::Add),
    BlendModeDesc(true,  BlendFactor::SrcAlpha    , BlendFactor::InvSrcAlpha, BlendOp::Add        , BlendFactor::SrcAlpha    , BlendFactor::InvSrcAlpha, BlendOp::Add),
    BlendModeDesc(true,  BlendFactor::SrcAlpha    , BlendFactor::One        , BlendOp::Add        , BlendFactor::SrcAlpha    , BlendFactor::One        , BlendOp::Add),
    BlendModeDesc(true,  BlendFactor::One         , BlendFactor::InvSrcAlpha, BlendOp::Add        , BlendFactor::One         , BlendFactor::InvSrcAlpha, BlendOp::Add),
    BlendModeDesc(true,  BlendFactor::InvDestAlpha, BlendFactor::DestAlpha  , BlendOp::Add        , BlendFactor::InvDestAlpha, BlendFactor::DestAlpha  , BlendOp::Add),
    BlendModeDesc(true,  BlendFactor::One         , BlendFactor::One        , BlendOp::RevSubtract, BlendFactor::One         , BlendFactor::One        , BlendOp::RevSubtract),
    BlendModeDesc(true,  BlendFactor::SrcAlpha    , BlendFactor::One        , BlendOp::RevSubtract, BlendFactor::SrcAlpha    , BlendFactor::One        , BlendOp::RevSubtract)
};

void samplerFilterToGL(SamplerFilter filter, GLenum &glFilter, bool &glIsAnisotropic, bool &glIsComparison) {
    switch(filter) {
        case SamplerFilter::Point: {
            glIsAnisotropic = false;
            glIsComparison = false;
            glFilter = GL_NEAREST;
            break;
        }
        case SamplerFilter::Linear: {
            glIsAnisotropic = false;
            glIsComparison = false;
            glFilter = GL_LINEAR;
            break;
        }
        case SamplerFilter::Anisotropic: {
            glIsAnisotropic = true;
            glIsComparison = false;
            glFilter = GL_LINEAR;
            break;
        }
        case SamplerFilter::ComparisonPoint: {
            glIsAnisotropic = false;
            glIsComparison = true;
            glFilter = GL_NEAREST;
            break;
        }
        case SamplerFilter::ComparisonLinear: {
            glIsAnisotropic = false;
            glIsComparison = true;
            glFilter = GL_LINEAR;
            break;
        }
        case SamplerFilter::ComparisonAnisotropic: {
            glIsAnisotropic = true;
            glIsComparison = true;
            glFilter = GL_LINEAR;
            break;
        }
    }
}

void debugCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char *message, const void *userParam) {
    const char *sourceStr = nullptr, *typeStr = nullptr, *severityStr = nullptr;
    switch (source) {
        case GL_DEBUG_SOURCE_API_ARB: {
            sourceStr = "API";
            break;
        }
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: {
            sourceStr = "Window system";
            break;
        }
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: {
            sourceStr = "Shader compiler";
            break;
        }
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: {
            sourceStr = "Third party";
            break;
        }
        case GL_DEBUG_SOURCE_APPLICATION_ARB: {
            sourceStr = "Application";
            break;
        }
        case GL_DEBUG_SOURCE_OTHER_ARB: {
            sourceStr = "Other";
            break;
        }
        default: {
            sourceStr = "Unknown";
            break;
        }
    }
    switch (type) {
        case GL_DEBUG_TYPE_ERROR_ARB: {
            typeStr = "Error";
            break;
        }
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: {
            typeStr = "Deprecated behaviour";
            break;
        }
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: {
            typeStr = "Undefined behaviour";
            break;
        }
        case GL_DEBUG_TYPE_PORTABILITY_ARB: {
            typeStr = "Portability";
            break;
        }
        case GL_DEBUG_TYPE_PERFORMANCE_ARB: {
            typeStr = "Performance";
            break;
        }
        case GL_DEBUG_TYPE_OTHER_ARB: {
            typeStr = "Other";
            break;
        }
        default: {
            typeStr = "Unknown";
            break;
        }
    }
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH_ARB: {
            severityStr = "High";
            break;
        }
        case GL_DEBUG_SEVERITY_MEDIUM_ARB: {
            severityStr = "Medium";
            break;
        }
        case GL_DEBUG_SEVERITY_LOW_ARB: {
            severityStr = "Low";
            break;
        }
        default: {
            severityStr = "Unknown";
            break;
        }
    }
    if (type == GL_DEBUG_TYPE_ERROR_ARB) {
        HD_LOG_ERROR("OpenGL Debug:\n\tSource: {}\n\tType: {}\n\tId: {}\n\tSeverity: {}\n\tMessage: {}", sourceStr, typeStr, id, severityStr, message);
    }
    else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB || type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB || type == GL_DEBUG_TYPE_PORTABILITY_ARB || type == GL_DEBUG_TYPE_PERFORMANCE_ARB) {
        HD_LOG_WARNING("OpenGL Debug:\n\tSource: {}\n\tType: {}\n\tId: {}\n\tSeverity: {}\n\tMessage: {}", sourceStr, typeStr, id, severityStr, message);
    }
    else {
        HD_LOG_INFO("OpenGL Debug:\n\tSource: {}\n\tType: {}\n\tId: {}\n\tSeverity: {}\n\tMessage: {}", sourceStr, typeStr, id, severityStr, message);
    }
}

VertexElement::VertexElement() {
    this->type = VertexElementType::Float;
    this->index = 0;
    this->slot = 0;
    this->offset = 0;
    this->normalized = false;
    this->perInstance = false;
}

VertexElement::VertexElement(VertexElementType type, uint32_t index, uint32_t slot, uint32_t offset, bool normalized, bool perInstance) {
    this->type = type;
    this->index = index;
    this->slot = slot;
    this->offset = offset;
    this->normalized = normalized;
    this->perInstance = perInstance;
}

BlendModeDesc::BlendModeDesc() {
    this->blendEnable = false;
    this->srcBlend = BlendFactor::One;
    this->dstBlend = BlendFactor::One;
    this->blendOp = BlendOp::Add;
    this->srcBlendAlpha = BlendFactor::One;
    this->dstBlendAlpha = BlendFactor::One;
    this->blendOpAlpha = BlendOp::Add;
}

BlendModeDesc::BlendModeDesc(bool blendEnable, BlendFactor srcBlend, BlendFactor dstBlend, BlendOp blendOp, BlendFactor srcBlendAlpha, BlendFactor dstBlendAlpha, BlendOp blendOpAlpha) {
    this->blendEnable = blendEnable;
    this->srcBlend = srcBlend;
    this->dstBlend = dstBlend;
    this->blendOp = blendOp;
    this->srcBlendAlpha = srcBlendAlpha;
    this->dstBlendAlpha = dstBlendAlpha;
    this->blendOpAlpha = blendOpAlpha;
}

StencilTestDesc::StencilTestDesc() {
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
}

StencilTestDesc::StencilTestDesc(CompareFunc frontFunc, StencilOp frontFail, StencilOp frontDepthFail, StencilOp frontPass,
                                 CompareFunc backFunc, StencilOp backFail, StencilOp backDepthFail, StencilOp backPass,
                                 uint32_t readMask, uint32_t writeMask){
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
}

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

bool ColorMask::operator==(const ColorMask &rhs) const {
    return this->r == rhs.r && this->g == rhs.g && this->b == rhs.b && this->a == rhs.a;
}

bool ColorMask::operator!=(const ColorMask &rhs) const {
    return this->r != rhs.r || this->g != rhs.g || this->b != rhs.b || this->a != rhs.a;
}

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

bool PolygonOffset::operator==(const PolygonOffset &rhs) const {
    return this->enabled == rhs.enabled && hd::MathUtils::compareFloat(this->factor, rhs.factor) && hd::MathUtils::compareFloat(this->units, rhs.units);
}

bool PolygonOffset::operator!=(const PolygonOffset &rhs) const {
    return this->enabled != rhs.enabled || !hd::MathUtils::compareFloat(this->factor, rhs.factor) || !hd::MathUtils::compareFloat(this->units, rhs.units);
}

SamplerStateDesc::SamplerStateDesc() : borderColor{0.0f, 0.0f, 0.0f, 0.0f} {
    this->minFilter = SamplerFilter::Linear;
    this->magFilter = SamplerFilter::Linear;
    this->mipFilter = SamplerFilter::Linear;
    this->u = SamplerAddressMode::Clamp;
    this->v = SamplerAddressMode::Clamp;
    this->w = SamplerAddressMode::Clamp;
    this->maxAnisotropy = 0;
    this->mipLodBias = 0.0f;
    this->compareFunc = CompareFunc::Never;
    this->minLod = 0.0f;
    this->maxLod = FLT_MAX;
}

struct VertexFormatImpl {
    uint32_t id;
    std::vector<VertexElement> elements;
};

struct VertexBufferImpl {
    uint32_t id, size;
    BufferUsage usage;
};

struct IndexBufferImpl {
    uint32_t id, size;
    BufferUsage usage;
};

struct ConstantBufferImpl {
    uint32_t id, size;
    BufferUsage usage;
};

struct Texture2DImpl {
    uint32_t id;
    TextureFormat format;
    glm::ivec2 size;
};

struct Texture2DArrayImpl {
    uint32_t id;
    TextureFormat format;
    glm::ivec2 size;
    uint32_t layers;
};

struct SamplerStateImpl {
    uint32_t id;
    SamplerStateDesc desc;
};

struct ProgramImpl {
    uint32_t id;
    std::string name, vsCode, psCode, defines;
};

RenderDevice::RenderDevice() {
}

void RenderDevice::initialize() {
    glewExperimental = true;
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        HD_LOG_ERROR("Failed to initialize GLEW. Errors: {}", glewGetErrorString(error));
    }

    if (GLEW_ARB_debug_output) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
    }
    else {
        HD_LOG_WARNING("Failed to enable OpenGL debug output. Extension 'GLEW_ARB_debug_output' not supported");
    }
}

void RenderDevice::shutdown() {
    for (auto &it : mVertexFormats) {
        destroyVertexFormat(it);
    }
    for (auto &it : mVertexBuffers) {
        destroyVertexBuffer(it);
    }
    for (auto &it : mIndexBuffers) {
        destroyIndexBuffer(it);
    }
    for (auto &it : mConstantBuffers) {
        destroyConstantBuffer(it);
    }
    for (auto &it : mTextures2D) {
        destroyTexture2D(it);
    }
    for (auto &it : mTexture2DArrays) {
        destroyTexture2DArray(it);
    }
    for (auto &it : mSamplerStates) {
        destroySamplerState(it);
    }
    for (auto &it : mPrograms) {
        destroyProgram(it);
    }
}

void RenderDevice::onEvent(const SDL_Event &event) {
    if (event.type == SDL_WINDOWEVENT && event.window.type == SDL_WINDOWEVENT_RESIZED) {
        setViewport(0, 0, event.window.data1, event.window.data2);
    }
}

void RenderDevice::clearRenderTarget(const glm::vec4 &rgba) {
    glClearColor(rgba.r, rgba.g, rgba.b, rgba.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderDevice::clearDepthStencil(float depth) {
    glClearDepth(depth);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void RenderDevice::draw(PrimitiveType primType, uint32_t vertexCount, uint32_t firstVertex) {
    glDrawArrays(gPrimitiveTypes[static_cast<uint32_t>(primType)], firstVertex, vertexCount);
}

void RenderDevice::drawInstanced(PrimitiveType primType, uint32_t vertexCountPerInstance, uint32_t firstVertex, uint32_t instanceCount) {
    glDrawArraysInstanced(gPrimitiveTypes[static_cast<uint32_t>(primType)], firstVertex, vertexCountPerInstance, instanceCount);
}

void RenderDevice::drawIndexed(PrimitiveType primType, uint32_t indexCount, uint32_t firstIndex, IndexType indexType) {
    switch (indexType) {
        case IndexType::Uint: {
            glDrawElements(gPrimitiveTypes[static_cast<uint32_t>(primType)], indexCount, GL_UNSIGNED_INT, static_cast<uint32_t*>(nullptr) + firstIndex);
            break;
        }
        case IndexType::Ushort: {
            glDrawElements(gPrimitiveTypes[static_cast<uint32_t>(primType)], indexCount, GL_UNSIGNED_SHORT, static_cast<uint16_t*>(nullptr) + firstIndex);
            break;
        }
        case IndexType::Ubyte: {
            glDrawElements(gPrimitiveTypes[static_cast<uint32_t>(primType)], indexCount, GL_UNSIGNED_BYTE, static_cast<uint8_t*>(nullptr) + firstIndex);
            break;
        }
    }
}

void RenderDevice::drawIndexedInstanced(PrimitiveType primType, uint32_t indexCountPerInstance, uint32_t firstIndex, IndexType indexType, uint32_t instanceCount) {
    switch (indexType) {
        case IndexType::Uint: {
            glDrawElementsInstanced(gPrimitiveTypes[static_cast<uint32_t>(primType)], indexCountPerInstance, GL_UNSIGNED_INT, static_cast<uint32_t*>(nullptr) + firstIndex, instanceCount);
            break;
        }
        case IndexType::Ushort: {
            glDrawElementsInstanced(gPrimitiveTypes[static_cast<uint32_t>(primType)], indexCountPerInstance, GL_UNSIGNED_SHORT, static_cast<uint16_t*>(nullptr) + firstIndex, instanceCount);
            break;
        }
        case IndexType::Ubyte: {
            glDrawElementsInstanced(gPrimitiveTypes[static_cast<uint32_t>(primType)], indexCountPerInstance, GL_UNSIGNED_BYTE, static_cast<uint8_t*>(nullptr) + firstIndex, instanceCount);
            break;
        }
    }
}

void RenderDevice::setDepthState(bool depthTestEnabled, CompareFunc compareFunc, bool depthMask) {
    if (depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
    glDepthFunc(gCompareFuncs[static_cast<size_t>(compareFunc)]);
    glDepthMask(depthMask);
}

void RenderDevice::setStencilState(bool enabled, const StencilTestDesc &desc, int ref) {
    if (enabled) {
        glEnable(GL_STENCIL_TEST);
    }
    else {
        glDisable(GL_STENCIL_TEST);
    }
    glStencilFuncSeparate(GL_FRONT, gCompareFuncs[static_cast<size_t>(desc.frontFunc)], ref, desc.readMask);
    glStencilFuncSeparate(GL_BACK, gCompareFuncs[static_cast<size_t>(desc.backFunc)], ref, desc.readMask);
    glStencilOpSeparate(GL_FRONT, gStencilOps[static_cast<size_t>(desc.frontFail)], gStencilOps[static_cast<size_t>(desc.frontDepthFail)], gStencilOps[static_cast<size_t>(desc.frontPass)]);
    glStencilOpSeparate(GL_BACK, gStencilOps[static_cast<size_t>(desc.backFail)], gStencilOps[static_cast<size_t>(desc.backDepthFail)], gStencilOps[static_cast<size_t>(desc.backPass)]);
    glStencilMask(desc.writeMask);
}

void RenderDevice::setBlendState(const BlendModeDesc &blendModeDesc, const ColorMask &colorMask) {
    if (blendModeDesc.blendEnable) {
        glEnable(GL_BLEND);
    }
    else {
        glDisable(GL_BLEND);
    }
    glBlendFuncSeparate(gBlendFactors[static_cast<size_t>(blendModeDesc.srcBlend)], gBlendFactors[static_cast<size_t>(blendModeDesc.dstBlend)],
        gBlendFactors[static_cast<size_t>(blendModeDesc.srcBlendAlpha)], gBlendFactors[static_cast<size_t>(blendModeDesc.dstBlendAlpha)]);
    glBlendEquationSeparate(gBlendOps[static_cast<size_t>(blendModeDesc.blendOp)], gBlendOps[static_cast<size_t>(blendModeDesc.blendOpAlpha)]);
    glColorMask(colorMask.r, colorMask.g, colorMask.b, colorMask.a);
}

void RenderDevice::setBlendState(BlendMode blendMode, const ColorMask &colorMask) {
    setBlendState(gBlendModeDescs[static_cast<uint32_t>(blendMode)], colorMask);
}

void RenderDevice::setRasterizerState(CullFace cullFace, FillMode fillMode, FrontFace frontFace, const PolygonOffset &polygonOffset) {
    if (cullFace == CullFace::None) {
        glDisable(GL_CULL_FACE);
    }
    else {
        glEnable(GL_CULL_FACE);
        glCullFace(gCullFaces[static_cast<size_t>(cullFace)]);
    }
    glPolygonMode(GL_FRONT_AND_BACK, gFillModes[static_cast<size_t>(fillMode)]);
    glFrontFace(gFrontFaces[static_cast<size_t>(frontFace)]);
    if (polygonOffset.enabled) {
        glEnable(GL_POLYGON_OFFSET_FILL);
    }
    else {
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
    glPolygonOffset(polygonOffset.factor, polygonOffset.units);
}

void RenderDevice::setViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

HVertexFormat RenderDevice::createVertexFormat(const std::vector<VertexElement> &desc) {
    HD_ASSERT(!desc.empty());
    VertexFormatImpl *obj = new VertexFormatImpl();
    mVertexFormats.push_back(HVertexFormat(obj));
    obj->elements = desc;
    glGenVertexArrays(1, &obj->id);
    glBindVertexArray(obj->id);
    for (const auto &it : desc) {
        glEnableVertexAttribArray(it.index);
        glVertexAttribBinding(it.index, it.slot);
        glVertexAttribFormat(it.index, gVertexElementSizes[static_cast<size_t>(it.type)], gVertexElementTypes[static_cast<size_t>(it.type)], it.normalized, it.offset);
        glVertexBindingDivisor(it.slot, it.perInstance);
    }
    return mVertexFormats.back();
}

void RenderDevice::destroyVertexFormat(HVertexFormat &handle) {
    if (handle) {
        mVertexFormats.erase(std::remove(mVertexFormats.begin(), mVertexFormats.end(), handle), mVertexFormats.end());
        glDeleteVertexArrays(1, &handle->id);
        HD_DELETE(handle.value);
        handle.invalidate();
    }
}

void RenderDevice::bindVertexFormat(const HVertexFormat &handle) {
    HD_ASSERT(handle);
    glBindVertexArray(handle->id);
}

HVertexBuffer RenderDevice::createVertexBuffer(const void *data, uint32_t size, BufferUsage usage) {
    HD_ASSERT(size != 0);
    VertexBufferImpl *obj = new VertexBufferImpl();
    mVertexBuffers.push_back(HVertexBuffer(obj));
    obj->size = size;
    obj->usage = usage;
    glGenBuffers(1, &obj->id);
    setVertexBufferData(mVertexBuffers.back(), data, size);
    return mVertexBuffers.back();
}

void RenderDevice::destroyVertexBuffer(HVertexBuffer &handle) {
    if (handle) {
        mVertexBuffers.erase(std::remove(mVertexBuffers.begin(), mVertexBuffers.end(), handle), mVertexBuffers.end());
        glDeleteBuffers(1, &handle->id);
        HD_DELETE(handle.value);
        handle.invalidate();
    }
}

void RenderDevice::setVertexBufferData(HVertexBuffer &handle, const void *data, uint32_t size) {
    HD_ASSERT(handle);
    HD_ASSERT(size != 0);
    handle->size = size;
    glBindBuffer(GL_ARRAY_BUFFER, handle->id);
    glBufferData(GL_ARRAY_BUFFER, size, data, gBufferUsages[static_cast<size_t>(handle->usage)]);
}

void RenderDevice::updateVertexBufferData(HVertexBuffer &handle, const void *data) {
    HD_ASSERT(handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle->id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, handle->size, data);
}

void RenderDevice::updateVertexBufferData(HVertexBuffer &handle, const void *data, uint32_t offset, uint32_t size) {
    HD_ASSERT(handle);
    HD_ASSERT(size != 0);
    HD_ASSERT((offset + size) <= handle->size);
    glBindBuffer(GL_ARRAY_BUFFER, handle->id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void *RenderDevice::mapVertexBuffer(HVertexBuffer &handle, BufferAccess access) {
    HD_ASSERT(handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle->id);
    return glMapBuffer(GL_ARRAY_BUFFER, gBufferAccesses[static_cast<size_t>(access)]);
}

void RenderDevice::unmapVertexBuffer(HVertexBuffer &handle) {
    HD_ASSERT(handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle->id);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void RenderDevice::bindVertexBuffer(const HVertexBuffer &handle, uint32_t slot, uint32_t offset, uint32_t stride) {
    HD_ASSERT(handle);
    HD_ASSERT(slot < MAX_VERTEX_BUFFERS);
    HD_ASSERT(stride != 0);
    glBindVertexBuffer(slot, handle->id, offset, stride);
}

HIndexBuffer RenderDevice::createIndexBuffer(const void *data, uint32_t size, BufferUsage usage) {
    HD_ASSERT(size != 0);
    IndexBufferImpl *obj = new IndexBufferImpl();
    mIndexBuffers.push_back(HIndexBuffer(obj));
    obj->size = size;
    obj->usage = usage;
    glGenBuffers(1, &obj->id);
    setIndexBufferData(mIndexBuffers.back(), data, size);
    return mIndexBuffers.back();
}

void RenderDevice::destroyIndexBuffer(HIndexBuffer &handle) {
    if (handle) {
        mIndexBuffers.erase(std::remove(mIndexBuffers.begin(), mIndexBuffers.end(), handle), mIndexBuffers.end());
        glDeleteBuffers(1, &handle->id);
        HD_DELETE(handle.value);
        handle.invalidate();
    }
}

void RenderDevice::setIndexBufferData(HIndexBuffer &handle, const void *data, uint32_t size) {
    HD_ASSERT(handle);
    HD_ASSERT(size != 0);
    handle->size = size;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, gBufferUsages[static_cast<size_t>(handle->usage)]);
}

void RenderDevice::updateIndexBufferData(HIndexBuffer &handle, const void *data) {
    HD_ASSERT(handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle->id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, handle->size, data);
}

void RenderDevice::updateIndexBufferData(HIndexBuffer &handle, const void *data, uint32_t offset, uint32_t size) {
    HD_ASSERT(handle);
    HD_ASSERT(size != 0);
    HD_ASSERT((offset + size) <= handle->size);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle->id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void *RenderDevice::mapIndexBuffer(HIndexBuffer &handle, BufferAccess access) {
    HD_ASSERT(handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle->id);
    return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, gBufferAccesses[static_cast<size_t>(access)]);
}

void RenderDevice::unmapIndexBuffer(HIndexBuffer &handle) {
    HD_ASSERT(handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle->id);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void RenderDevice::bindIndexBuffer(const HIndexBuffer &handle) {
    HD_ASSERT(handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle->id);
}

HConstantBuffer RenderDevice::createConstantBuffer(const void *data, uint32_t size, BufferUsage usage) {
    HD_ASSERT(size != 0);
    ConstantBufferImpl *obj = new ConstantBufferImpl();
    mConstantBuffers.push_back(HConstantBuffer(obj));
    obj->size = size;
    obj->usage = usage;
    glGenBuffers(1, &obj->id);
    setConstantBufferData(mConstantBuffers.back(), data, size);
    return mConstantBuffers.back();
}

void RenderDevice::destroyConstantBuffer(HConstantBuffer &handle) {
    if (handle) {
        mConstantBuffers.erase(std::remove(mConstantBuffers.begin(), mConstantBuffers.end(), handle), mConstantBuffers.end());
        glDeleteBuffers(1, &handle->id);
        HD_DELETE(handle.value);
        handle.invalidate();
    }
}

void RenderDevice::setConstantBufferData(HConstantBuffer &handle, const void *data, uint32_t size) {
    HD_ASSERT(handle);
    HD_ASSERT(size != 0);
    handle->size = size;
    glBindBuffer(GL_UNIFORM_BUFFER, handle->id);
    glBufferData(GL_UNIFORM_BUFFER, size, data, gBufferUsages[static_cast<size_t>(handle->usage)]);
}

void RenderDevice::updateConstantBufferData(HConstantBuffer &handle, const void *data) {
    HD_ASSERT(handle);
    glBindBuffer(GL_UNIFORM_BUFFER, handle->id);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, handle->size, data);
}

void RenderDevice::updateConstantBufferData(HConstantBuffer &handle, const void *data, uint32_t offset, uint32_t size) {
    HD_ASSERT(handle);
    HD_ASSERT(size != 0);
    HD_ASSERT((offset + size) <= handle->size);
    glBindBuffer(GL_UNIFORM_BUFFER, handle->id);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void *RenderDevice::mapConstantBuffer(HConstantBuffer &handle, BufferAccess access) {
    HD_ASSERT(handle);
    glBindBuffer(GL_UNIFORM_BUFFER, handle->id);
    return glMapBuffer(GL_UNIFORM_BUFFER, gBufferAccesses[static_cast<size_t>(access)]);
}

void RenderDevice::unmapConstantBuffer(HConstantBuffer &handle) {
    HD_ASSERT(handle);
    glBindBuffer(GL_UNIFORM_BUFFER, handle->id);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void RenderDevice::bindConstantBuffer(const HConstantBuffer &handle, uint32_t slot) {
    HD_ASSERT(handle);
    HD_ASSERT(slot < MAX_CONSTANT_BUFFERS);
    glBindBufferBase(GL_UNIFORM_BUFFER, slot, handle->id);
}

HTexture2D RenderDevice::createTexture2D(const void *data, const glm::ivec2 &size, TextureFormat format) {
    HD_ASSERT(size.x != 0 && size.y != 0);
    Texture2DImpl *obj = new Texture2DImpl();
    mTextures2D.push_back(HTexture2D(obj));
    obj->format = format;
    obj->size = size;
    glGenTextures(1, &obj->id);
    glBindTexture(GL_TEXTURE_2D, obj->id);
    glTexImage2D(GL_TEXTURE_2D, 0, gTextureInternalFormats[static_cast<size_t>(format)], size.x, size.y, 0, gTextureExternalFormats[static_cast<size_t>(format)], gTextureDataTypes[static_cast<size_t>(format)], data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return mTextures2D.back();
}

HTexture2D RenderDevice::createTexture2DFromStream(hd::Stream &stream) {
    hd::Image img(stream);
    return createTexture2D(img.getData(), img.getSize(), TextureFormat::RGBA8);
}

HTexture2D RenderDevice::createTexture2DFromFile(const std::string &filename) {
    hd::FileStream fs(getEngine().getCreateInfo().texturesDataPath + filename, hd::FileMode::Read);
    fs.setName(filename);
    return createTexture2DFromStream(fs);
}

void RenderDevice::destroyTexture2D(HTexture2D &handle) {
    if (handle) {
        mTextures2D.erase(std::remove(mTextures2D.begin(), mTextures2D.end(), handle), mTextures2D.end());
        glDeleteTextures(1, &handle->id);
        HD_DELETE(handle.value);
        handle.invalidate();
    }
}

void RenderDevice::bindTexture2D(const HTexture2D &handle, uint32_t slot) {
    HD_ASSERT(handle);
    HD_ASSERT(slot < MAX_TEXTURES);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, handle->id);
}

HTexture2DArray RenderDevice::createTexture2DArray(const void *data, const glm::ivec2 &size, TextureFormat format, uint32_t layers) {
    HD_ASSERT(size.x != 0 && size.y != 0);
    Texture2DArrayImpl *obj = new Texture2DArrayImpl();
    mTexture2DArrays.push_back(HTexture2DArray(obj));
    obj->format = format;
    obj->size = size;
    glGenTextures(1, &obj->id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, obj->id);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, gTextureInternalFormats[static_cast<size_t>(format)], size.x, size.y, layers, 0, gTextureExternalFormats[static_cast<size_t>(format)], gTextureDataTypes[static_cast<size_t>(format)], data);
    return mTexture2DArrays.back();
}

HTexture2DArray RenderDevice::createTexture2DArrayFromFiles(const std::vector<std::string> &filenames) {
    HD_ASSERT(!filenames.empty());
    std::vector<hd::Image> images;
    images.reserve(filenames.size());
    for (const auto &filename : filenames) {
        images.emplace_back(getEngine().getCreateInfo().texturesDataPath + filename);
        if (images.size() > 1 && images[0].getSize().x != images[1].getSize().x && images[0].getSize().y != images[1].getSize().y) {
            HD_LOG_ERROR("Failed to create Texture2DArray from files:\n{}\nThe files has a different image sizes", hd::StringUtils::unite(filenames, "'", "'", "\n").data());
        }
    }
    HTexture2DArray handle = createTexture2DArray(nullptr, images.front().getSize(), TextureFormat::RGBA8, static_cast<uint32_t>(filenames.size()));
    for (size_t i = 0; i < images.size(); i++) {
        setTexture2DArrayLayerData(handle, static_cast<uint32_t>(i), images[i].getData(), TextureFormat::RGBA8);
    }
    return handle;
}

void RenderDevice::setTexture2DArrayLayerData(const HTexture2DArray &handle, uint32_t layer, const void *data, TextureFormat format) {
   HD_ASSERT(handle);
   glBindTexture(GL_TEXTURE_2D_ARRAY, handle->id);
   glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, handle->size.x, handle->size.y, 1, gTextureExternalFormats[static_cast<size_t>(format)], gTextureDataTypes[static_cast<size_t>(format)], data);
   glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void RenderDevice::setTexture2DArrayLayerData(const HTexture2DArray &handle, uint32_t layer, hd::Stream &stream) {
    hd::Image img(stream);
    setTexture2DArrayLayerData(handle, layer, img.getData(), TextureFormat::RGBA8);
}

void RenderDevice::setTexture2DArrayLayerData(const HTexture2DArray &handle, uint32_t layer, const std::string &filename) {
    hd::FileStream fs(filename, hd::FileMode::Read);
    fs.setName(filename);
    setTexture2DArrayLayerData(handle, layer, fs);
}

void RenderDevice::destroyTexture2DArray(HTexture2DArray &handle) {
    if (handle) {
        mTexture2DArrays.erase(std::remove(mTexture2DArrays.begin(), mTexture2DArrays.end(), handle), mTexture2DArrays.end());
        glDeleteTextures(1, &handle->id);
        HD_DELETE(handle.value);
        handle.invalidate();
    }
}

void RenderDevice::bindTexture2DArray(const HTexture2DArray &handle, uint32_t slot) {
    HD_ASSERT(handle);
    HD_ASSERT(slot < MAX_TEXTURES);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, handle->id);
}

HSamplerState RenderDevice::createSamplerState(const SamplerStateDesc& desc) {
    SamplerStateImpl *obj = new SamplerStateImpl();
    mSamplerStates.push_back(HSamplerState(obj));
    obj->desc = desc;

    bool isMinAniso = false, isMagAniso = false, isMipAniso = false;
    bool isMinComp = false, isMagComp = false, isMipComp = false;
    GLenum minFilter, magFilter, mipFilter;
    samplerFilterToGL(desc.minFilter, minFilter, isMinAniso, isMinComp);
    samplerFilterToGL(desc.magFilter, magFilter, isMagAniso, isMagComp);
    samplerFilterToGL(desc.mipFilter, mipFilter, isMipAniso, isMipComp);
    HD_ASSERT(isMinAniso == isMagAniso && isMagAniso == isMipAniso);
    HD_ASSERT(isMinComp == isMagComp && isMagComp == isMipComp);

    GLenum minMipFilter = 0;
    if(minFilter == GL_NEAREST && mipFilter == GL_NEAREST) {
        minMipFilter = GL_NEAREST_MIPMAP_NEAREST;
    }
    else if(minFilter == GL_LINEAR && mipFilter == GL_NEAREST) {
        minMipFilter = GL_LINEAR_MIPMAP_NEAREST;
    }
    else if(minFilter == GL_NEAREST && mipFilter == GL_LINEAR) {
        minMipFilter = GL_NEAREST_MIPMAP_LINEAR;
    }
    else if(minFilter == GL_LINEAR && mipFilter == GL_LINEAR) {
        minMipFilter = GL_LINEAR_MIPMAP_LINEAR;
    }
    else {
        HD_LOG_ERROR("Unsupported min/mip filter combination");
    }

    glGenSamplers(1, &obj->id);
    glSamplerParameteri(obj->id, GL_TEXTURE_MIN_FILTER, minMipFilter);
    glSamplerParameteri(obj->id, GL_TEXTURE_MAG_FILTER, magFilter);
    glSamplerParameteri(obj->id, GL_TEXTURE_MAX_ANISOTROPY_EXT, isMipAniso ? desc.maxAnisotropy : 1);
    glSamplerParameteri(obj->id, GL_TEXTURE_COMPARE_MODE, isMinComp ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE);
    glSamplerParameteri(obj->id, GL_TEXTURE_COMPARE_FUNC, gCompareFuncs[static_cast<GLenum>(desc.compareFunc)]);
    glSamplerParameteri(obj->id, GL_TEXTURE_WRAP_S, gSamplerAddressModes[static_cast<size_t>(desc.u)]);
    glSamplerParameteri(obj->id, GL_TEXTURE_WRAP_T, gSamplerAddressModes[static_cast<size_t>(desc.v)]);
    glSamplerParameteri(obj->id, GL_TEXTURE_WRAP_R, gSamplerAddressModes[static_cast<size_t>(desc.w)]);
    glSamplerParameterf(obj->id, GL_TEXTURE_LOD_BIAS, desc.mipLodBias);
    glSamplerParameterfv(obj->id, GL_TEXTURE_BORDER_COLOR, desc.borderColor);
    glSamplerParameterf(obj->id, GL_TEXTURE_MIN_LOD, desc.minLod);
    glSamplerParameterf(obj->id, GL_TEXTURE_MAX_LOD, desc.maxLod);
    return mSamplerStates.back();
}

void RenderDevice::destroySamplerState(HSamplerState &handle) {
    if (handle) {
        mSamplerStates.erase(std::remove(mSamplerStates.begin(), mSamplerStates.end(), handle), mSamplerStates.end());
        glDeleteSamplers(1, &handle->id);
        HD_DELETE(handle.value);
        handle.invalidate();
    }
}

void RenderDevice::bindSamplerState(const HSamplerState &handle, uint32_t slot) {
    HD_ASSERT(handle);
    HD_ASSERT(slot < MAX_SAMPLER_STATES);
    glBindSampler(slot, handle->id);
}

HProgram RenderDevice::createProgram(const std::string &name, const std::string &vsCode, const std::string &psCode, const std::string &defines) {
    HD_ASSERT(!vsCode.empty());
    HD_ASSERT(!psCode.empty());
    ProgramImpl *obj = new ProgramImpl();
    mPrograms.push_back(HProgram(obj));
    obj->name = name;
    obj->vsCode = vsCode;
    obj->psCode = psCode;
    obj->defines = defines;
    obj->id = glCreateProgram();

    int result = 0, logLength = 0;
    std::string versionStr = "#version 330 core\n";
    std::string definesStr;
    if (!defines.empty()) {
        for (const auto &it : hd::StringUtils::split(defines, ";", false)) {
            definesStr += "#define" + it + "\n";
        }
    }

    std::string vsSource = versionStr + definesStr + "#line 1\n" + vsCode;
    std::string psSource = versionStr + definesStr + "#line 1\n" + psCode;

    uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
    const char *vsSources[] = { vsSource.data() };
    int vsSourcesLengths[] = { static_cast<int>(vsSource.length()) };
    glShaderSource(vs, HD_ARRAYSIZE(vsSourcesLengths), vsSources, vsSourcesLengths);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> errorStr(static_cast<size_t>(logLength));
        glGetShaderInfoLog(vs, logLength, nullptr, errorStr.data());
        HD_LOG_ERROR("Failed to compile vertex shader for program '{}'. Errors:\n{}", name.data(), errorStr.data());
    }

    uint32_t ps = glCreateShader(GL_FRAGMENT_SHADER);
    const char *psSources[] = { psSource.data() };
    int psSourcesLengths[] = { static_cast<int>(psSource.length()) };
    glShaderSource(ps, HD_ARRAYSIZE(psSourcesLengths), psSources, psSourcesLengths);
    glCompileShader(ps);
    glGetShaderiv(ps, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderiv(ps, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> errorStr(static_cast<size_t>(logLength));
        glGetShaderInfoLog(ps, logLength, nullptr, errorStr.data());
        HD_LOG_ERROR("Failed to compile pixel shader for program '{}'. Errors:\n{}", name.data(), errorStr.data());
    }

    glAttachShader(obj->id, vs);
    glAttachShader(obj->id, ps);
    glLinkProgram(obj->id);
    glGetProgramiv(obj->id, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramiv(obj->id, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> errorStr(static_cast<size_t>(logLength));
        glGetProgramInfoLog(obj->id, logLength, nullptr, errorStr.data());
        HD_LOG_ERROR("Failed to link program '{}'. Errors:\n{}", name.data(), errorStr.data());
    }

    glValidateProgram(obj->id);
    glGetProgramiv(obj->id, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramiv(obj->id, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> errorStr(static_cast<size_t>(logLength));
        glGetProgramInfoLog(obj->id, logLength, nullptr, errorStr.data());
        HD_LOG_ERROR("Failed to link program '{}'. Errors:\n{}", name.data(), errorStr.data());
    }

    glDetachShader(obj->id, vs);
    glDeleteShader(vs);
    glDetachShader(obj->id, ps);
    glDeleteShader(ps);
    return mPrograms.back();
}

HProgram RenderDevice::createProgramFromStream(hd::Stream &stream, const std::string &defines) {
    std::vector<std::string> text = hd::StringUtils::split(stream.readAllText(), "\n", false);
    std::string code[2];
    int shaderType = 0; // 0 - vs, 1 - ps;
    for (const auto &it : text) {
        if (hd::StringUtils::contains(it, "@VertexShader", false)) {
            shaderType = 0;
        }
        else if (hd::StringUtils::contains(it, "@PixelShader", false)) {
            shaderType = 1;
        }
        else {
            code[shaderType] += it + "\n";
        }
    }
    return createProgram(stream.getName(), code[0], code[1], defines);
}

HProgram RenderDevice::createProgramFromFile(const std::string &filename, const std::string &defines) {
    hd::FileStream fs(filename, hd::FileMode::Read);
    fs.setName(filename);
    return createProgramFromStream(fs, defines);
}

void RenderDevice::destroyProgram(HProgram &handle) {
    if (handle) {
        mPrograms.erase(std::remove(mPrograms.begin(), mPrograms.end(), handle), mPrograms.end());
        glDeleteProgram(handle->id);
        HD_DELETE(handle.value);
        handle.invalidate();
    }
}

HProgramConstant RenderDevice::getProgramConstantID(const HProgram &handle, const std::string &name) {
    HD_ASSERT(handle);
    HD_ASSERT(!name.empty());
    return HProgramConstant(glGetUniformLocation(handle->id, name.data()));
}

void RenderDevice::bindProgram(const HProgram &handle) {
    HD_ASSERT(handle);
    glUseProgram(handle->id);
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, int value) {
    HD_ASSERT(id);
    glUniform1i(id.value, value);
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, float value) {
    HD_ASSERT(id);
    glUniform1f(id.value, value);
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, const glm::vec2 &value) {
    HD_ASSERT(id);
    glUniform2f(id.value, value.x, value.y);
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, const glm::vec3 &value) {
    HD_ASSERT(id);
    glUniform3f(id.value, value.x, value.y, value.z);
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, const glm::vec4 &value) {
    HD_ASSERT(id);
    glUniform4f(id.value, value.x, value.y, value.z, value.w);
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, const glm::mat4 &value) {
    HD_ASSERT(id);
    glUniformMatrix4fv(id.value, 1, false, reinterpret_cast<const float*>(&value));
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, const int *value, uint32_t count) {
    HD_ASSERT(id);
    glUniform1iv(id.value, count, value);
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, const float *value, uint32_t count) {
    HD_ASSERT(id);
    glUniform1fv(id.value, count, value);
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, const glm::vec2 *value, uint32_t count) {
    HD_ASSERT(id);
    glUniform2fv(id.value, count, reinterpret_cast<const float*>(value));
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, const glm::vec3 *value, uint32_t count) {
    HD_ASSERT(id);
    glUniform3fv(id.value, count, reinterpret_cast<const float*>(value));
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, const glm::vec4 *value, uint32_t count) {
    HD_ASSERT(id);
    glUniform4fv(id.value, count, reinterpret_cast<const float*>(value));
}

void RenderDevice::setProgramConstant(const HProgramConstant &id, const glm::mat4 *value, uint32_t count) {
    HD_ASSERT(id);
    glUniformMatrix4fv(id.value, count, false, reinterpret_cast<const float*>(value));
}

}
