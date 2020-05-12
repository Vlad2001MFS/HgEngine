#pragma once
#include "VertexFormat.hpp"
#include "Buffer.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "Texture2D.hpp"
#include "DepthStencilState.hpp"
#include "RasterizerState.hpp"
#include "BlendState.hpp"
#include "hd/Core/StringHash.hpp"
#include <glm/glm.hpp>
#include <unordered_map>

namespace hg {

enum class PrimitiveType {
    Points = GL_POINTS,
    LineStrip = GL_LINE_STRIP,
    Lines = GL_LINES,
    TriangleStrip = GL_TRIANGLE_STRIP,
    Triangles = GL_TRIANGLES
};

enum class IndexType {
    Uint32 = GL_UNSIGNED_INT,
    Uint16 = GL_UNSIGNED_SHORT,
    Uint8 = GL_UNSIGNED_BYTE
};

enum class BlendMode {
    Replace,
    Add,
    Multiply,
    Alpha,
    AddAlpha,
    PreMulAlpha,
    InvDestAlpha,
    Subtract,
    SubtractAlpha,
};

class RenderDevice {
public:
    RenderDevice();
    ~RenderDevice();

    void clearRenderTarget(const glm::vec4 &rgba);
    void clearDepthStencil(float depth, uint8_t stencil);
    void draw(PrimitiveType primType, uint32_t firstVertex, uint32_t vertexCount);
    void drawInstanced(PrimitiveType primType, uint32_t firstVertex, uint32_t vertexCountPerInstance, uint32_t instanceCount);
    void drawIndexed(PrimitiveType primType, uint32_t indexCount, IndexType indexType, uint32_t firstIndex);
    void drawIndexedInstanced(PrimitiveType primType, uint32_t indexCountPerInstance, IndexType indexType, uint32_t firstIndex, uint32_t instanceCount);
    void setViewport(int x, int y, int w, int h);

    void setVertexFormat(const VertexFormatPtr &obj);
    void setVertexBuffer(const BufferPtr &obj, uint32_t slot, uint32_t offset, uint32_t stride);
    void setIndexBuffer(const BufferPtr &obj);
    void setConstantBuffer(const BufferPtr &obj, uint32_t slot);
    void setVertexShader(const VertexShaderPtr &obj);
    void setPixelShader(const PixelShaderPtr &obj);
    void setTexture(const TexturePtr &obj, uint32_t slot);
    void setDepthStencilState(const DepthStencilStatePtr &obj);
    void setBlendState(const BlendStatePtr &obj);
    void setBlendState(BlendMode mode);
    void setRasterizerState(const RasterizerStatePtr &obj);

    Texture2DPtr loadTexture2D(const std::string &path);

    static const uint32_t MAX_VERTEXBUFFERS = 8;
    static const uint32_t MAX_CONSTANTBUFFERS = 8;
    static const uint32_t MAX_TEXTURES = 16;

private:
    void mPrepareDraw();

    VertexFormatPtr mCurrentVF;
    bool mCurrentVFIsDirty;

    BufferPtr mCurrentVB[MAX_VERTEXBUFFERS];
    uint32_t mCurrentVBOffset[MAX_VERTEXBUFFERS];
    uint32_t mCurrentVBStride[MAX_VERTEXBUFFERS];
    bool mCurrentVBIsDirty[MAX_VERTEXBUFFERS];

    BufferPtr mCurrentIB;
    bool mCurrentIBIsDirty;

    BufferPtr mCurrentCB[MAX_CONSTANTBUFFERS];
    VertexShaderPtr mCurrentVS;
    PixelShaderPtr mCurrentPS;
    TexturePtr mCurrentTex[MAX_TEXTURES];

    DepthStencilStatePtr mCurrentDSS;
    BlendStatePtr mCurrentBS;
    RasterizerStatePtr mCurrentRS;

    std::unordered_map<BlendMode, BlendStatePtr> mBlendStateModes;
    uint32_t mProgramPipeline;
    bool mProgramPipelineIsDirty;

    std::unordered_map<hd::StringHash, Texture2DPtr> mTextures2D;
};

RenderDevice &getRenderDevice();

}
