#pragma once
#include "Texture.hpp"
#include <string>

namespace hg {

using Texture2DPtr = std::shared_ptr<class Texture2D>;

class Texture2D : public Texture {
public:
    Texture2D(uint32_t id, const glm::ivec2 &size, TextureFormat format);

    const glm::ivec2 &getSize() const;

    static Texture2DPtr create(const void *data, const glm::ivec2 &size, TextureFormat format);
    static Texture2DPtr createFromFile(const std::string &path);

private:
    glm::ivec2 mSize;
};

}