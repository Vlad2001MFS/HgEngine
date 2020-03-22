#include "VertexFormat.hpp"
#include "magic_enum/magic_enum.hpp"
#include "GLEW/glew.h"

namespace hg {

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

VertexAttrib::VertexAttrib(AttribType type, uint32_t index, uint32_t slot, uint32_t offset, bool normalized, bool perInstance) {
    this->type = type;
    this->index = index;
    this->slot = slot;
    this->offset = offset;
    this->normalized = normalized;
    this->perInstance = perInstance;
}

VertexFormat::VertexFormat(uint32_t id, const std::vector<VertexAttrib> &attribs) : mAttribs(attribs) {
    mId = id;
}

VertexFormat::~VertexFormat() {
    glDeleteVertexArrays(1, &mId);
}

uint32_t VertexFormat::getId() const {
    return mId;
}

const std::vector<VertexAttrib> &VertexFormat::getAttribs() const {
    return mAttribs;
}

VertexFormatPtr VertexFormat::create(const std::vector<VertexAttrib> &attribs) {
    uint32_t id;
    glCreateVertexArrays(1, &id);
    for (const auto &it : attribs) {
        glEnableVertexArrayAttrib(id, it.index);
        glVertexArrayAttribBinding(id, it.index, it.slot);
        glVertexArrayAttribFormat(id, it.index, gVertexElementSizes[static_cast<size_t>(it.type)], gVertexElementTypes[static_cast<size_t>(it.type)], it.normalized, it.offset);
        glVertexArrayBindingDivisor(id, it.slot, it.perInstance);
    }
    
    return std::make_shared<VertexFormat>(id, attribs);
}

}