#pragma once
#include <GL/glew.h>

namespace hg {

enum class CompareFunc {
    Never = GL_NEVER,
    Less = GL_LESS,
    Equal = GL_EQUAL,
    LessEqual = GL_LEQUAL,
    Greater = GL_GREATER,
    NotEqual = GL_NOTEQUAL,
    GreatEqual = GL_GEQUAL,
    Always = GL_ALWAYS
};

}