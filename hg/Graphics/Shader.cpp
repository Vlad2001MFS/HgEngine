#include "Shader.hpp"
#include "hd/Core/Log.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/IO/FileStream.hpp"
#include "GLEW/glew.h"
#include "magic_enum/magic_enum.hpp"

namespace hg {

Shader::Shader(ShaderType type, uint32_t id, const std::string &source, const std::vector<std::pair<std::string, std::string>> &defines)
    : mSource(source), mDefines(defines) {
    mType = type;
    mId = id;
}

Shader::~Shader() {
    glDeleteProgram(mId);
}

void Shader::setUniform(const UniformProxy &idOrName, int value) {
    glProgramUniform1i(mId, mGetIdFromProxy(idOrName), value);
}

void Shader::setUniform(const UniformProxy &idOrName, float value) {
    glProgramUniform1f(mId, mGetIdFromProxy(idOrName), value);
}

void Shader::setUniform(const UniformProxy &idOrName, const glm::vec2 &value) {
    glProgramUniform2f(mId, mGetIdFromProxy(idOrName), value.x, value.y);
}

void Shader::setUniform(const UniformProxy &idOrName, const glm::vec3 &value) {
    glProgramUniform3f(mId, mGetIdFromProxy(idOrName), value.x, value.y, value.z);
}

void Shader::setUniform(const UniformProxy &idOrName, const glm::vec4 &value) {
    glProgramUniform4f(mId, mGetIdFromProxy(idOrName), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const UniformProxy &idOrName, const glm::mat4 &value) {
    glProgramUniformMatrix4fv(mId, mGetIdFromProxy(idOrName), 1, false, reinterpret_cast<const float*>(&value));
}

void Shader::setUniform(const UniformProxy &idOrName, const int *value, uint32_t count) {
    glProgramUniform1iv(mId, mGetIdFromProxy(idOrName), count, value);
}

void Shader::setUniform(const UniformProxy &idOrName, const float *value, uint32_t count) {
    glProgramUniform1fv(mId, mGetIdFromProxy(idOrName), count, value);
}

void Shader::setUniform(const UniformProxy &idOrName, const glm::vec2 *value, uint32_t count) {
    glProgramUniform2fv(mId, mGetIdFromProxy(idOrName), count, reinterpret_cast<const float*>(value));
}

void Shader::setUniform(const UniformProxy &idOrName, const glm::vec3 *value, uint32_t count) {
    glProgramUniform3fv(mId, mGetIdFromProxy(idOrName), count, reinterpret_cast<const float*>(value));
}

void Shader::setUniform(const UniformProxy &idOrName, const glm::vec4 *value, uint32_t count) {
    glProgramUniform4fv(mId, mGetIdFromProxy(idOrName), count, reinterpret_cast<const float*>(value));
}

void Shader::setUniform(const UniformProxy &idOrName, const glm::mat4 *value, uint32_t count) {
    glProgramUniformMatrix4fv(mId, mGetIdFromProxy(idOrName), count, false, reinterpret_cast<const float*>(value));
}

UniformID Shader::getUniformID(std::string_view name) const {
    int id = glGetUniformLocation(mId, name.data());
    if (id < 0) {
        HD_LOG_ERROR("Failed to get ID of uniform '{}' from {} {}", name, magic_enum::enum_name(mType), mId);
    }
    return UniformID(id);
}

ShaderType Shader::getType() const {
    return mType;
}

uint32_t Shader::getId() const {
    return mId;
}

const std::string &Shader::getSource() const {
    return mSource;
}

const std::vector<std::pair<std::string, std::string>> &Shader::getDefines() const {
    return mDefines;
}

constexpr int Shader::mGetIdFromProxy(const UniformProxy &idOrName) const {
    auto func = [&](auto &&arg) -> int {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, UniformID>) {
            return arg.value;
        }
        else if constexpr (std::is_same_v<T, std::string_view>) {
            return getUniformID(arg).value;
        }
        else {
            static_assert(true, "mGetIdFromProxy failed");
            return -1;
        }
    };
    return std::visit(func, idOrName);
}

}