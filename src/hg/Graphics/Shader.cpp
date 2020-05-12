#include "Shader.hpp"
#include "../Core/Engine.hpp"
#include "hd/Core/Log.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/IO/FileStream.hpp"
#include "../../nameof/nameof.hpp"
#include <GL/glew.h>

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
        HD_LOG_ERROR("Failed to get ID of uniform '{}' from {} {}", name, NAMEOF_ENUM(mType), mId);
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

std::string Shader::mGetFullPath(const std::string &path) {
    return "./data/shaders/" + path;
}

std::string Shader::mGetFullSrc(const std::string &src, const std::vector<std::pair<std::string, std::string>> &defines) {
    std::string text = hd::StringUtils::removeSymbols(src, "\r", false);
    std::vector<std::string> lines = hd::StringUtils::split(text, "\n", false);
    for (auto &line : lines) {
        std::string data = hd::StringUtils::beforeFirst(line, '/');
        if (hd::StringUtils::contains(data, "#version", false)) {
            line = data + "\n";
            for (const auto &it : defines) {
                line += "#define" + it.first + " " + it.second + "\n";
            }
            line += "#line 2";
            break;
        }
    }

    return hd::StringUtils::unite(lines, "", "", "\n");
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
