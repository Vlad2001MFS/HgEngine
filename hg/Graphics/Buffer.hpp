#pragma once
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

using BufferPtr = std::shared_ptr<class Buffer>;

class Buffer {
public:
    Buffer(uint32_t id, uint32_t size, BufferUsage usage);
    ~Buffer();

    void update(const void *data, uint32_t offset = 0, uint32_t size = 0);

    uint32_t getId() const;
    uint32_t getSize() const;
    BufferUsage getUsage() const;

    static BufferPtr create(const void *data, uint32_t size, BufferUsage usage = BufferUsage::None);

private:
    uint32_t mId;
    uint32_t mSize;
    BufferUsage mUsage;
};

}