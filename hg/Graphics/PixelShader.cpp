#include "PixelShader.hpp"
#include "hd/Core/Log.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/IO/FileStream.hpp"
#include "GLEW/glew.h"

namespace hg {

PixelShader::PixelShader(uint32_t id, const std::string &source, const std::vector<std::pair<std::string, std::string>> &defines)
    : Shader(ShaderType::Pixel, id, source, defines) {
}

PixelShaderPtr PixelShader::create(const std::string &source, const std::vector<std::pair<std::string, std::string>> &defines) {
    std::string src;
    std::vector<std::string> lines = hd::StringUtils::split(source, "\n", false);
    for (auto &line : lines) {
        src += line + "\n";
        if (hd::StringUtils::contains(hd::StringUtils::beforeFirst(line, '/'), "#version", true)) {
            for (const auto &it : defines) {
                src += "#define" + it.first + " " + it.second + "\n";
            }
        }
    }

    const char *srcPtr[] = { src.data() };
    uint32_t id = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, srcPtr);

    int status;
    glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
    if (!status) {
        int len;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
        std::string log(len + 1, '\0');
        glGetProgramInfoLog(id, len, nullptr, log.data());
        HD_LOG_FATAL("Failed to compile pixel shader:\n{}", log.data());
    }

    return std::make_shared<PixelShader>(id, source, defines);
}

PixelShaderPtr PixelShader::createFromFile(const std::string &path, const std::vector<std::pair<std::string, std::string>> &defines) {
    return create(hd::FileStream(path, hd::FileMode::Read).readAllText(), defines);
}

}