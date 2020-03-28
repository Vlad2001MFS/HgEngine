#include "Texture2D.hpp"

namespace hg {

Texture2D::Texture2D(uint32_t id, const glm::ivec2 &size, TextureFormat format) : Texture(id, format, TextureType::Tex2D) {
    mSize = size;
}

const glm::ivec2 &Texture2D::getSize() const {
    return mSize;
}

Texture2DPtr Texture2D::create(const void *data, const glm::ivec2 &size, TextureFormat format) {
    uint32_t id;
    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    glTextureStorage2D(id, 1, mGetTextureInternalFormat(format), size.x, size.y);
    glTextureSubImage2D(id, 0, 0, 0, size.x, size.y, mGetTextureExternalFormat(format), mGetTextureDataType(format), data);
    glGenerateTextureMipmap(id);

    return std::make_shared<Texture2D>(id, size, format);
}

Texture2DPtr Texture2D::createFromFile(const std::string &path) {
    auto image = hd::Image(path, hd::ImageFormat::None, true);
    return create(image.getData(), image.getSize(), mGetTextureFormatFromImageFormat(image.getFormat()));
}

}
