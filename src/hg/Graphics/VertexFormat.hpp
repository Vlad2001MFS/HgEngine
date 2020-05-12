#pragma once
#include <vector>
#include <memory>

namespace hg {

enum class AttribType {
    Float,
    Float2,
    Float3,
    Float4
};

struct VertexAttrib {
    VertexAttrib(AttribType type, uint32_t index, uint32_t slot, uint32_t offset, bool normalized, bool perInstance);

    AttribType type;
    uint32_t index, slot, offset;
    bool normalized, perInstance;
};

using VertexFormatPtr = std::shared_ptr<class VertexFormat>;

class VertexFormat {
public:
    VertexFormat(uint32_t id, const std::vector<VertexAttrib> &attribs);
    ~VertexFormat();

    uint32_t getId() const;
    const std::vector<VertexAttrib> &getAttribs() const;

    static VertexFormatPtr create(const std::vector<VertexAttrib> &attribs);

private:
    uint32_t mId;
    std::vector<VertexAttrib> mAttribs;
};

}