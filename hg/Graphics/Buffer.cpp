#include "Buffer.hpp"
#include "magic_enum/magic_enum.hpp"

namespace hg {

Buffer::Buffer(uint32_t id, uint32_t size, BufferUsage usage) {
    mId = id;
    mSize = size;
    mUsage = usage;
}

Buffer::~Buffer() {
    glDeleteBuffers(1, &mId);
}

void Buffer::update(const void *data, uint32_t offset, uint32_t size) {
    glNamedBufferSubData(mId, offset, size != 0 ? size : mSize, data);
}

uint32_t Buffer::getId() const {
    return mId;
}

uint32_t Buffer::getSize() const {
    return mSize;
}

BufferUsage Buffer::getUsage() const {
    return mUsage;
}

BufferPtr Buffer::create(const void *data, uint32_t size, BufferUsage usage) {
    uint32_t id;
    glCreateBuffers(1, &id);
    glNamedBufferStorage(id, size, data, static_cast<GLenum>(usage));

    return std::make_shared<Buffer>(id, size, usage);
}

}