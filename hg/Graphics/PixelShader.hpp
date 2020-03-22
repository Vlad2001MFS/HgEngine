#pragma once
#include "Shader.hpp"
#include <memory>

namespace hg {

using PixelShaderPtr = std::shared_ptr<class PixelShader>;

class PixelShader : public Shader {
public:
    PixelShader(uint32_t id, const std::string &source, const std::vector<std::pair<std::string, std::string>> &defines);

    static PixelShaderPtr create(const std::string &source, const std::vector<std::pair<std::string, std::string>> &defines = {});
    static PixelShaderPtr createFromFile(const std::string &path, const std::vector<std::pair<std::string, std::string>> &defines = {});
};

}