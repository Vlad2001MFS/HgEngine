#include "Texture2D.hpp"

namespace hg {

Texture2D::Texture2D(uint32_t id, const glm::ivec2 &size, TextureFormat format) : Texture(id, format, TextureType::Tex2D) {
    mSize = size;
}

const glm::ivec2 &Texture2D::getSize() const {
    return mSize;
}

const std::string &Texture2D::getPath() const {
    return mPath;
}

Texture2DPtr Texture2D::create(const void *data, const glm::ivec2 &size, TextureFormat format) {
    uint32_t id;
    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    glTextureStorage2D(id, 1, mGetTextureInternalFormat(format), size.x, size.y);
    glTextureSubImage2D(id, 0, 0, 0, size.x, size.y, mGetTextureExternalFormat(format), mGetTextureDataType(format), data);
    glGenerateTextureMipmap(id);

    return std::make_shared<Texture2D>(id, size, format);
}

Texture2DPtr Texture2D::createFromColor(const glm::vec4 &color) {
    return create(&color, glm::ivec2(1, 1), TextureFormat::RGBA8);
}

Texture2DPtr Texture2D::createFromImage(const hd::Image &image) {
    return create(image.getData(), image.getSize(), mGetTextureFormatFromImageFormat(image.getFormat()));
}

Texture2DPtr Texture2D::createFromFile(const std::string &path) {
    Texture2DPtr tex = createFromImage(hd::Image(mGetFullPath(path), hd::ImageFormat::None, false));
    tex->mPath = path;
    return tex;
}

}
