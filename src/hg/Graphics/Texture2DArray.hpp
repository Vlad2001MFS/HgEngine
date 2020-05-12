#pragma once
#include "Texture.hpp"
#include "hd/IO/Image.hpp"
#include <string>
#include <vector>

namespace hg {

using Texture2DArrayPtr = std::shared_ptr<class Texture2DArray>;

class Texture2DArray : public Texture {
public:
    Texture2DArray(uint32_t id, const glm::ivec2 &size, TextureFormat format, uint32_t layers);

    const glm::ivec2 &getSize() const;
    uint32_t getLayers() const;

    static Texture2DArrayPtr create(const void **data, uint32_t layers, const glm::ivec2 &size, TextureFormat format);
    static Texture2DArrayPtr createFromImages(const std::vector<hd::Image> &images);
    static Texture2DArrayPtr createFromFiles(const std::vector<std::string> &paths);

private:
    glm::ivec2 mSize;
    uint32_t mLayers;
};

}
