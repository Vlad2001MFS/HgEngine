#include "Texture.hpp"
#include "hd/Core/Log.hpp"
#include "glm/ext.hpp"
#include "magic_enum/magic_enum.hpp"

namespace hg {

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

Texture::Texture(uint32_t id, TextureFormat format, TextureType type) {
    mId = id;
    mFormat = format;
    mType = type;

    mMinFilter = TextureFilter::NearestMipmapLinear;
    mMagFilter = TextureFilter::Linear;
    mU = TextureAddressMode::Repeat;
    mV = TextureAddressMode::Repeat;
    mW = TextureAddressMode::Repeat;
    mAnisotropy = 1;
    mMipLodBias = 0.0f;
    mCompareFunc = CompareFunc::Less;
    mCompareMode = false;
    mBorderColor = glm::vec4(0, 0, 0, 0);
    mMinLod = -1000;
    mMaxLod = 1000;
}

Texture::~Texture() {
    glDeleteTextures(1, &mId);
}

void Texture::setMinFilter(TextureFilter filter) {
    if (mMinFilter != filter) {
        mMinFilter = filter;
        glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(filter));
    }
}

void Texture::setMagFilter(TextureFilter filter) {
    if (mMagFilter != filter) {
        mMagFilter = filter;
        glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filter));
    }
}

void Texture::setAddressModeU(TextureAddressMode u) {
    if (mU != u) {
        mU = u;
        glTextureParameteri(mId, GL_TEXTURE_WRAP_S, static_cast<GLenum>(u));
    }
}

void Texture::setAddressModeV(TextureAddressMode v) {
    if (mV != v) {
        mV = v;
        glTextureParameteri(mId, GL_TEXTURE_WRAP_T, static_cast<GLenum>(v));
    }
}

void Texture::setAddressModeW(TextureAddressMode w) {
    if (mW != w) {
        mW = w;
        glTextureParameteri(mId, GL_TEXTURE_WRAP_R, static_cast<GLenum>(w));
    }
}

void Texture::setAnisotropy(uint32_t anisotropy) {
    if (mAnisotropy != anisotropy) {
        mAnisotropy = anisotropy;
        glTextureParameteri(mId, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);
    }
}

void Texture::setMipLodBias(float mipLodBias) {
    if (mMipLodBias != mipLodBias) {
        mMipLodBias = mipLodBias;
        glTextureParameterf(mId, GL_TEXTURE_LOD_BIAS, mipLodBias);
    }
}

void Texture::setCompareFunc(CompareFunc func) {
    if (mCompareFunc != func) {
        mCompareFunc = func;
        glTextureParameteri(mId, GL_TEXTURE_COMPARE_FUNC, static_cast<GLenum>(func));
    }
}

void Texture::setCompareMode(bool mode) {
    if (mCompareMode != mode) {
        mCompareMode = mode;
        glTextureParameteri(mId, GL_TEXTURE_COMPARE_MODE, mode);
    }
}

void Texture::setBorderColor(const glm::vec4 &color) {
    if (mBorderColor != color) {
        mBorderColor = color;
        glTextureParameterfv(mId, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
    }
}

void Texture::setMinLod(float minLod) {
    if (mMinLod != minLod) {
        mMinLod = minLod;
        glTextureParameterf(mId, GL_TEXTURE_MIN_LOD, minLod);
    }
}

void Texture::setMaxLod(float maxLod) {
    if (mMaxLod != maxLod) {
        mMaxLod = maxLod;
        glTextureParameterf(mId, GL_TEXTURE_MAX_LOD, maxLod);
    }
}

uint32_t Texture::getId() const {
    return mId;
}

TextureFormat Texture::getFormat() const {
    return mFormat;
}

TextureType Texture::getType() const {
    return mType;
}

int Texture::mGetTextureInternalFormat(TextureFormat fmt) {
    return gTextureInternalFormats[static_cast<size_t>(fmt)];
}

GLenum Texture::mGetTextureExternalFormat(TextureFormat fmt) {
    return gTextureExternalFormats[static_cast<size_t>(fmt)];
}

GLenum Texture::mGetTextureDataType(TextureFormat fmt) {
    return gTextureDataTypes[static_cast<size_t>(fmt)];
}

TextureFormat Texture::mGetTextureFormatFromImageFormat(hd::ImageFormat fmt) {
    switch (fmt) {
        case hd::ImageFormat::Grey: {
            return TextureFormat::R8;
        }
        case hd::ImageFormat::RGB: {
            return TextureFormat::RGB8;
        }
        case hd::ImageFormat::RGBA: {
            return TextureFormat::RGBA8;
        }
        default: {
            HD_LOG_FATAL("Invalid image format: {}({})", magic_enum::enum_name(fmt), fmt);
        }
    }
}

}