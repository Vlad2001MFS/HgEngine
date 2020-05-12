#include "RenderDevice.hpp"
#include "../Core/Engine.hpp"
#include "hd/Core/Log.hpp"

namespace hg {

static void debugCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char *msg, const void *data) {
    const char *sourceStr = nullptr, *typeStr = nullptr, *severityStr = nullptr;
    switch (source) {
        case GL_DEBUG_SOURCE_API_ARB:             sourceStr = "API";             break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:   sourceStr = "Window system";   break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: sourceStr = "Shader compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:     sourceStr = "Third party";     break;
        case GL_DEBUG_SOURCE_APPLICATION_ARB:     sourceStr = "Application";     break;
        case GL_DEBUG_SOURCE_OTHER_ARB:           sourceStr = "Other";           break;
        default:                                  sourceStr = "Unknown";         break;
    }
    switch (type) {
        case GL_DEBUG_TYPE_ERROR_ARB:               typeStr = "Error";                break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: typeStr = "Deprecated behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:  typeStr = "Undefined behaviour";  break;
        case GL_DEBUG_TYPE_PORTABILITY_ARB:         typeStr = "Portability";          break;
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:         typeStr = "Performance";          break;
        case GL_DEBUG_TYPE_OTHER_ARB:               typeStr = "Other";                break;
        default:                                    typeStr = "Unknown";              break;
    }
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH_ARB:   severityStr = "High";    break;
        case GL_DEBUG_SEVERITY_MEDIUM_ARB: severityStr = "Medium";  break;
        case GL_DEBUG_SEVERITY_LOW_ARB:    severityStr = "Low";     break;
        default:                           severityStr = "Unknown"; break;
    }

    constexpr auto fmt = "OpenGL Debug:\n\tSource: {}\n\tType: {}\n\tId: {}\n\tSeverity: {}\n\tMessage: {}";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR_ARB: {
            HD_LOG_ERROR(fmt, sourceStr, typeStr, id, severityStr, msg);
            break;
        }
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        case GL_DEBUG_TYPE_PORTABILITY_ARB:
        case GL_DEBUG_TYPE_PERFORMANCE_ARB: {
            HD_LOG_WARNING(fmt, sourceStr, typeStr, id, severityStr, msg);
            break;
        }
        default: {
            HD_LOG_INFO(fmt, sourceStr, typeStr, id, severityStr, msg);
            break;
        }
    }
}

RenderDevice::RenderDevice() : mCurrentVBIsDirty{false}, mCurrentVBOffset{0}, mCurrentVBStride{0} {
    mCurrentVFIsDirty = false;
    mCurrentIBIsDirty = false;
    mProgramPipelineIsDirty = false;

    int majorVer = getEngine().getCreateInfo().glMajorVer;
    int minorVer = getEngine().getCreateInfo().glMinorVer;
    if (majorVer != 4 || minorVer != 5) {
        HD_LOG_ERROR("OpenGL Context does not satisfy to minimum required 4.5 version. Used context {}.{} version", majorVer, minorVer);
    }

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

    mBlendStateModes[BlendMode::Replace      ] = BlendState::create({false, BlendFactor::One         , BlendFactor::Zero       , BlendOp::Add        , BlendFactor::One         , BlendFactor::Zero       , BlendOp::Add        , {true, true, true, true}});
    mBlendStateModes[BlendMode::Add          ] = BlendState::create({true,  BlendFactor::One         , BlendFactor::One        , BlendOp::Add        , BlendFactor::One         , BlendFactor::One        , BlendOp::Add        , {true, true, true, true}});
    mBlendStateModes[BlendMode::Multiply     ] = BlendState::create({true,  BlendFactor::DestColor   , BlendFactor::Zero       , BlendOp::Add        , BlendFactor::One         , BlendFactor::Zero       , BlendOp::Add        , {true, true, true, true}});
    mBlendStateModes[BlendMode::Alpha        ] = BlendState::create({true,  BlendFactor::SrcAlpha    , BlendFactor::InvSrcAlpha, BlendOp::Add        , BlendFactor::SrcAlpha    , BlendFactor::InvSrcAlpha, BlendOp::Add        , {true, true, true, true}});
    mBlendStateModes[BlendMode::AddAlpha     ] = BlendState::create({true,  BlendFactor::SrcAlpha    , BlendFactor::One        , BlendOp::Add        , BlendFactor::SrcAlpha    , BlendFactor::One        , BlendOp::Add        , {true, true, true, true}});
    mBlendStateModes[BlendMode::PreMulAlpha  ] = BlendState::create({true,  BlendFactor::One         , BlendFactor::InvSrcAlpha, BlendOp::Add        , BlendFactor::One         , BlendFactor::InvSrcAlpha, BlendOp::Add        , {true, true, true, true}});
    mBlendStateModes[BlendMode::InvDestAlpha ] = BlendState::create({true,  BlendFactor::InvDestAlpha, BlendFactor::DestAlpha  , BlendOp::Add        , BlendFactor::InvDestAlpha, BlendFactor::DestAlpha  , BlendOp::Add        , {true, true, true, true}});
    mBlendStateModes[BlendMode::Subtract     ] = BlendState::create({true,  BlendFactor::One         , BlendFactor::One        , BlendOp::RevSubtract, BlendFactor::One         , BlendFactor::One        , BlendOp::RevSubtract, {true, true, true, true}});
    mBlendStateModes[BlendMode::SubtractAlpha] = BlendState::create({true,  BlendFactor::SrcAlpha    , BlendFactor::One        , BlendOp::RevSubtract, BlendFactor::SrcAlpha    , BlendFactor::One        , BlendOp::RevSubtract, {true, true, true, true}});

    glCreateProgramPipelines(1, &mProgramPipeline);
    glBindProgramPipeline(mProgramPipeline);
}

RenderDevice::~RenderDevice() {
    glDeleteProgramPipelines(1, &mProgramPipeline);
}

void RenderDevice::clearRenderTarget(const glm::vec4 &rgba) {
    glClearColor(rgba.r, rgba.g, rgba.b, rgba.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderDevice::clearDepthStencil(float depth, uint8_t stencil) {
    glClearDepth(depth);
    glClearStencil(stencil);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RenderDevice::draw(PrimitiveType primType, uint32_t firstVertex, uint32_t vertexCount) {
    mPrepareDraw();
    glDrawArrays(static_cast<GLenum>(primType), firstVertex, vertexCount);
}

void RenderDevice::drawInstanced(PrimitiveType primType, uint32_t firstVertex, uint32_t vertexCountPerInstance, uint32_t instanceCount) {
    mPrepareDraw();
    glDrawArraysInstanced(static_cast<GLenum>(primType), firstVertex, vertexCountPerInstance, instanceCount);
}

void RenderDevice::drawIndexed(PrimitiveType primType, uint32_t indexCount, IndexType indexType, uint32_t firstIndex) {
    mPrepareDraw();
    glDrawElements(static_cast<GLenum>(primType), indexCount, static_cast<GLenum>(indexType), reinterpret_cast<uint32_t*>(firstIndex));
}

void RenderDevice::drawIndexedInstanced(PrimitiveType primType, uint32_t indexCountPerInstance, IndexType indexType, uint32_t firstIndex, uint32_t instanceCount) {
    mPrepareDraw();
    glDrawElementsInstanced(static_cast<GLenum>(primType), indexCountPerInstance, static_cast<GLenum>(indexType),
                            static_cast<uint32_t*>(nullptr) + firstIndex, instanceCount);
}

void RenderDevice::setViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

void RenderDevice::setVertexFormat(const VertexFormatPtr &obj) {
    if (mCurrentVF != obj) {
        mCurrentVF = obj;
        mCurrentVFIsDirty = true;
        for (auto &it : mCurrentVBIsDirty) {
            it = true;
        }
        mCurrentIBIsDirty = true;
    }
}

void RenderDevice::setVertexBuffer(const BufferPtr &obj, uint32_t slot, uint32_t offset, uint32_t stride) {
    if (slot < MAX_VERTEXBUFFERS && (mCurrentVB[slot] != obj || mCurrentVBOffset[slot] != offset || mCurrentVBStride[slot] != stride)) {
        mCurrentVB[slot] = obj;
        mCurrentVBOffset[slot] = offset;
        mCurrentVBStride[slot] = stride;
        mCurrentVBIsDirty[slot] = true;
    }
}

void RenderDevice::setIndexBuffer(const BufferPtr &obj) {
    if (mCurrentIB != obj) {
        mCurrentIB = obj;
        mCurrentIBIsDirty = true;
    }
}

void RenderDevice::setConstantBuffer(const BufferPtr &obj, uint32_t slot) {
    if (slot < MAX_CONSTANTBUFFERS && mCurrentCB[slot] != obj) {
        mCurrentCB[slot] = obj;
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, obj ? obj->getId() : 0);
    }
}

void RenderDevice::setVertexShader(const VertexShaderPtr &obj) {
    if (mCurrentVS != obj) {
        mCurrentVS = obj;
        glUseProgramStages(mProgramPipeline, GL_VERTEX_SHADER_BIT, obj ? obj->getId() : 0);
        mProgramPipelineIsDirty = true;
    }
}

void RenderDevice::setPixelShader(const PixelShaderPtr &obj) {
    if (mCurrentPS != obj) {
        mCurrentPS = obj;
        glUseProgramStages(mProgramPipeline, GL_FRAGMENT_SHADER_BIT, obj ? obj->getId() : 0);
        mProgramPipelineIsDirty = true;
    }
}

void RenderDevice::setTexture(const TexturePtr &obj, uint32_t slot) {
    if (slot < MAX_TEXTURES && mCurrentTex[slot] != obj) {
        mCurrentTex[slot] = obj;
        glBindTextureUnit(slot, obj ? obj->getId() : 0);
    }
}

void RenderDevice::setDepthStencilState(const DepthStencilStatePtr &obj) {
    if (mCurrentDSS != obj) {
        mCurrentDSS = obj;
        auto depth = obj->getDesc().depth;
        if (depth.enabled) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(static_cast<GLenum>(depth.compareFunc));
            glDepthMask(depth.writeMask);
        }
        else {
            glDisable(GL_DEPTH_TEST);
        }

        auto stencil = obj->getDesc().stencil;
        if (stencil.enabled) {
            glEnable(GL_STENCIL_TEST);
            glStencilFuncSeparate(GL_FRONT, static_cast<GLenum>(stencil.frontFunc), stencil.refValue, stencil.readMask);
            glStencilFuncSeparate(GL_BACK, static_cast<GLenum>(stencil.backFunc), stencil.refValue, stencil.readMask);
            glStencilOpSeparate(GL_FRONT, static_cast<GLenum>(stencil.frontFail), static_cast<GLenum>(stencil.frontDepthFail), static_cast<GLenum>(stencil.frontPass));
            glStencilOpSeparate(GL_BACK, static_cast<GLenum>(stencil.backFail), static_cast<GLenum>(stencil.backDepthFail), static_cast<GLenum>(stencil.backPass));
            glStencilMask(stencil.writeMask);
        }
        else {
            glDisable(GL_STENCIL_TEST);
        }
    }
}

void RenderDevice::setBlendState(const BlendStatePtr &obj) {
    if (mCurrentBS != obj) {
        mCurrentBS = obj;
        auto desc = obj->getDesc();
        if (desc.enabled) {
            glEnable(GL_BLEND);
            glBlendFuncSeparate(static_cast<GLenum>(desc.srcFactor), static_cast<GLenum>(desc.dstFactor),
                                static_cast<GLenum>(desc.srcAlphaFactor), static_cast<GLenum>(desc.dstAlphaFactor));
            glBlendEquationSeparate(static_cast<GLenum>(desc.op), static_cast<GLenum>(desc.opAlpha));
        }
        else {
            glDisable(GL_BLEND);
        }
        glColorMask(desc.colorMask.r, desc.colorMask.g, desc.colorMask.b, desc.colorMask.a);
    }
}

void RenderDevice::setBlendState(BlendMode mode) {
    setBlendState(mBlendStateModes.at(mode));
}

void RenderDevice::setRasterizerState(const RasterizerStatePtr &obj) {
    if (mCurrentRS != obj) {
        mCurrentRS = obj;
        auto desc = obj->getDesc();
        if (desc.cullFace != CullFace::None) {
            glEnable(GL_CULL_FACE);
            glCullFace(static_cast<GLenum>(desc.cullFace));
            glFrontFace(static_cast<GLenum>(desc.frontFace));
        }
        else {
            glDisable(GL_CULL_FACE);
        }

        glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(desc.fillMode));

        if (desc.polygonOffset.enabled) {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(desc.polygonOffset.factor, desc.polygonOffset.units);
        }
        else {
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
    }
}

Texture2DPtr RenderDevice::loadTexture2D(const std::string &path) {
    if (!path.empty()) {
        hd::StringHash pathHash = hd::StringHash(path);
        if (mTextures2D.count(pathHash) == 0) {
            Texture2DPtr texture = Texture2D::createFromFile(path);
            mTextures2D.insert(std::make_pair(pathHash, texture));
            return texture;
        }
        else {
            return mTextures2D.at(pathHash);
        }
    }
    else {
        HD_LOG_FATAL("Failed to load texture. Path is empty");
        return Texture2DPtr();
    }
}

void RenderDevice::mPrepareDraw() {
    if (mCurrentVFIsDirty) {
        mCurrentVFIsDirty = false;
        glBindVertexArray(mCurrentVF ? mCurrentVF->getId() : 0);
    }
    for (uint32_t i = 0; i < MAX_VERTEXBUFFERS; i++) {
        if (mCurrentVBIsDirty[i]) {
            mCurrentVBIsDirty[i] = false;
            glVertexArrayVertexBuffer(mCurrentVF ? mCurrentVF->getId() : 0, i, mCurrentVB[i] ? mCurrentVB[i]->getId() : 0,
                mCurrentVBOffset[i], mCurrentVBStride[i]);
        }
    }
    if (mCurrentIBIsDirty) {
        mCurrentIBIsDirty = false;
        glVertexArrayElementBuffer(mCurrentVF ? mCurrentVF->getId() : 0, mCurrentIB ? mCurrentIB->getId() : 0);
    }
    if (mProgramPipelineIsDirty) {
        mProgramPipelineIsDirty = false;

        int status;
        glValidateProgramPipeline(mProgramPipeline);
        glGetProgramPipelineiv(mProgramPipeline, GL_VALIDATE_STATUS, &status);
        if (!status) {
            int len;
            glGetProgramPipelineiv(mProgramPipeline, GL_INFO_LOG_LENGTH, &len);

            std::string log(len + 1, '\0');
            glGetProgramPipelineInfoLog(mProgramPipeline, len + 1, NULL, log.data());

            HD_LOG_ERROR("Failed to validate OpenGL Program Pipeline:\n{}", log);
        }
    }
}

RenderDevice &getRenderDevice() {
    return getEngine().getRenderDevice();
}

}
