#include "Buffer.hpp"
#include "hd/Core/Log.hpp"
#include "magic_enum/magic_enum.hpp"

namespace hg {

Buffer::Buffer(uint32_t id, size_t size, BufferUsage usage) {
    mId = id;
    mSize = size;
    mUsage = usage;
}

Buffer::~Buffer() {
    glDeleteBuffers(1, &mId);
}

void Buffer::update(const void *data, size_t size, size_t offset) {
    if (offset != 0 && size == 0) {
        HD_LOG_ERROR("Invalid size({}) and offset({}) for call", size, offset);
    }
    glNamedBufferSubData(mId, offset, size != 0 ? size : mSize, data);
}

uint32_t Buffer::getId() const {
    return mId;
}

size_t Buffer::getSize() const {
    return mSize;
}

BufferUsage Buffer::getUsage() const {
    return mUsage;
}

BufferPtr Buffer::create(const void *data, size_t size, BufferUsage usage) {
    uint32_t id;
    glCreateBuffers(1, &id);
    glNamedBufferStorage(id, size, data, static_cast<GLenum>(usage));

    return std::make_shared<Buffer>(id, size, usage);
}

}