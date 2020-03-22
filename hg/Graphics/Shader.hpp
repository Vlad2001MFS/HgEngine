#pragma once
#include "hd/Core/Handle.hpp"
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <variant>

namespace hg {

enum class ShaderType {
    Vertex,
    Pixel
};

using UniformID = hd::Handle<int, class TAG_UniformID, -1>;

class Shader {
    using UniformProxy = std::variant<UniformID, std::string_view>;

public:
    void setUniform(const UniformProxy &idOrName, int value);
    void setUniform(const UniformProxy &idOrName, float value);
    void setUniform(const UniformProxy &idOrName, const glm::vec2 &value);
    void setUniform(const UniformProxy &idOrName, const glm::vec3 &value);
    void setUniform(const UniformProxy &idOrName, const glm::vec4 &value);
    void setUniform(const UniformProxy &idOrName, const glm::mat4 &value);
    void setUniform(const UniformProxy &idOrName, const int *value, uint32_t count);
    void setUniform(const UniformProxy &idOrName, const float *value, uint32_t count);
    void setUniform(const UniformProxy &idOrName, const glm::vec2 *value, uint32_t count);
    void setUniform(const UniformProxy &idOrName, const glm::vec3 *value, uint32_t count);
    void setUniform(const UniformProxy &idOrName, const glm::vec4 *value, uint32_t count);
    void setUniform(const UniformProxy &idOrName, const glm::mat4 *value, uint32_t count);

    UniformID getUniformID(std::string_view name) const;
    ShaderType getType() const;
    uint32_t getId() const;
    const std::string &getSource() const;
    const std::vector<std::pair<std::string, std::string>> &getDefines() const;

protected:
    Shader(ShaderType type, uint32_t id, const std::string &source, const std::vector<std::pair<std::string, std::string>> &defines);
    virtual ~Shader();

private:
    constexpr int mGetIdFromProxy(const UniformProxy &idOrName) const;

    ShaderType mType;
    uint32_t mId;
    std::string mSource;
    std::vector<std::pair<std::string, std::string>> mDefines;
};

}