#pragma once
#include "GraphicsDefs.hpp"
#include "hd/IO/Image.hpp"
#include "glm/glm.hpp"
#include <memory>

namespace hg {

enum class TextureFilter {
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
};

enum class TextureAddressMode {
    Repeat = GL_REPEAT,
    Mirror = GL_MIRRORED_REPEAT,
    Clamp = GL_CLAMP_TO_EDGE,
    Border = GL_CLAMP_TO_BORDER,
    MirrorOnce = GL_MIRROR_CLAMP_EXT
};

enum class TextureFormat {
    R8,
    RG8,
    RGB8,
    RGBA8,
    A8,
    R16,
    RG16,
    RGB16,
    RGBA16,
    R16F,
    RG16F,
    RGB16F,
    RGBA16F,
    R32F,
    RG32F,
    RGB32F,
    RGBA32F,
    D16,
    D24,
    D32,
    D24S8
};

enum class TextureType {
    Tex2D = GL_TEXTURE_2D
};

using TexturePtr = std::shared_ptr<class Texture>;

class Texture {
public:
    Texture(uint32_t id, TextureFormat format, TextureType type);
    virtual ~Texture();

    void setMinFilter(TextureFilter filter);
    void setMagFilter(TextureFilter filter);
    void setAddressModeU(TextureAddressMode u);
    void setAddressModeV(TextureAddressMode v);
    void setAddressModeW(TextureAddressMode w);
    void setAnisotropy(uint32_t anisotropy);
    void setMipLodBias(float mipLodBias);
    void setCompareFunc(CompareFunc func);
    void setCompareMode(bool mode);
    void setBorderColor(const glm::vec4 &color);
    void setMinLod(float minLod);
    void setMaxLod(float maxLod);

    uint32_t getId() const;
    TextureFormat getFormat() const;
    TextureType getType() const;

protected:
    static int mGetTextureInternalFormat(TextureFormat fmt);
    static GLenum mGetTextureExternalFormat(TextureFormat fmt);
    static GLenum mGetTextureDataType(TextureFormat fmt);
    static TextureFormat mGetTextureFormatFromImageFormat(hd::ImageFormat fmt);

private:
    uint32_t mId;
    TextureFormat mFormat;
    TextureType mType;

    TextureFilter mMinFilter;
    TextureFilter mMagFilter;
    TextureAddressMode mU;
    TextureAddressMode mV;
    TextureAddressMode mW;
    uint32_t mAnisotropy;
    float mMipLodBias;
    CompareFunc mCompareFunc;
    bool mCompareMode;
    glm::vec4 mBorderColor;
    float mMinLod;
    float mMaxLod;
};

}