#pragma once
#include "hd/Core/Common.hpp"
#include "GLEW/glew.h"
#include <memory>

namespace hg {

enum class BufferUsage {
    None = 0,
    DynamicStorage = GL_DYNAMIC_STORAGE_BIT,
    ClientStorage = GL_CLIENT_STORAGE_BIT,
    MapRead = GL_MAP_READ_BIT,
    MapWrite = GL_MAP_WRITE_BIT,
    MapPersistent = GL_MAP_PERSISTENT_BIT,
    MapCoherent = GL_MAP_COHERENT_BIT
};
HD_DECL_ENUM_OPS(BufferUsage);

enum class BufferAccess {
    Read = GL_MAP_READ_BIT,
    Write = GL_MAP_WRITE_BIT,
    Persistent = GL_MAP_PERSISTENT_BIT,
    Coherent = GL_MAP_COHERENT_BIT,
    InvalidateRange = GL_MAP_INVALIDATE_RANGE_BIT,
    InvalidateBuffer = GL_MAP_INVALIDATE_BUFFER_BIT,
    FlushExplicit = GL_MAP_FLUSH_EXPLICIT_BIT,
    Unsynchronized = GL_MAP_UNSYNCHRONIZED_BIT
};
HD_DECL_ENUM_OPS(BufferAccess);

using BufferPtr = std::shared_ptr<class Buffer>;

class Buffer {
public:
    Buffer(uint32_t id, size_t size, BufferUsage usage);
    ~Buffer();

    void update(const void *data, size_t size = 0, size_t offset = 0);
    void *map(BufferAccess access, size_t size = 0, size_t offset = 0);
    void unmap();

    uint32_t getId() const;
    size_t getSize() const;
    BufferUsage getUsage() const;

    static BufferPtr create(const void *data, size_t size, BufferUsage usage = BufferUsage::None);

private:
    uint32_t mId;
    size_t mSize;
    BufferUsage mUsage;
};

}