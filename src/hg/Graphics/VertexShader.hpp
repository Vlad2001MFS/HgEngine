#pragma once
#include "Shader.hpp"
#include <memory>

namespace hg {

using VertexShaderPtr = std::shared_ptr<class VertexShader>;

class VertexShader : public Shader {
public:
    VertexShader(uint32_t id, const std::string &source, const std::vector<std::pair<std::string, std::string>> &defines);

    static VertexShaderPtr create(const std::string &source, const std::vector<std::pair<std::string, std::string>> &defines = {});
    static VertexShaderPtr createFromFile(const std::string &path, const std::vector<std::pair<std::string, std::string>> &defines = {});
};

}
