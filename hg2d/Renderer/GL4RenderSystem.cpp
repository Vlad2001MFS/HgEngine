#include "RenderSystem.hpp"
#include "../Core/BuildConfig.hpp"

#ifdef HG2D_RENDERER_OPENGL4

#include "../Core/Engine.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/Core/Log.hpp"
#include "hd/Math/MathUtils.hpp"
#include "hd/IO/Image.hpp"
#include "hd/IO/FileStream.hpp"
#include "glm/ext.hpp"
#include "GLEW/glew.h"
#include <algorithm>
#pragma comment(lib, "opengl32.lib")

namespace hg2d {

struct Texture {
    uint32_t id;
    glm::ivec2 size;
    std::string path;
};

struct Vertex {
    float x, y, z;
    float u, v;
};

struct ConstantBuffer {
    glm::mat4 world;
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec2 uvOffset;
    glm::vec2 uvSize;
};

struct RenderSystem::Impl {
    Impl() {
        this->program = 0;
        this->vertexFormat = 0;
        this->vertexBuffer = 0;
        this->vertexBufferGUI = 0;
        this->constantBuffer = 0;
    }

    void destroyTexture(Texture *&obj) {
        glDeleteTextures(1, &obj->id);
        HD_DELETE(obj);
    }

    uint32_t program;
    uint32_t vertexFormat;
    uint32_t vertexBuffer;
    uint32_t vertexBufferGUI;
    uint32_t constantBuffer;

    std::vector<Texture*> createdTextures;
    ConstantBuffer cbData;
    glm::mat4 proj, projGUI;
};

void debugCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char *message, const void *userParam) {
    const char *sourceStr = nullptr, *typeStr = nullptr, *severityStr = nullptr;
    switch (source) {
        case GL_DEBUG_SOURCE_API_ARB: {
            sourceStr = "API";
            break;
        }
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: {
            sourceStr = "Window system";
            break;
        }
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: {
            sourceStr = "Shader compiler";
            break;
        }
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: {
            sourceStr = "Third party";
            break;
        }
        case GL_DEBUG_SOURCE_APPLICATION_ARB: {
            sourceStr = "Application";
            break;
        }
        case GL_DEBUG_SOURCE_OTHER_ARB: {
            sourceStr = "Other";
            break;
        }
        default: {
            sourceStr = "Unknown";
            break;
        }
    }
    switch (type) {
        case GL_DEBUG_TYPE_ERROR_ARB: {
            typeStr = "Error";
            break;
        }
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: {
            typeStr = "Deprecated behaviour";
            break;
        }
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: {
            typeStr = "Undefined behaviour";
            break;
        }
        case GL_DEBUG_TYPE_PORTABILITY_ARB: {
            typeStr = "Portability";
            break;
        }
        case GL_DEBUG_TYPE_PERFORMANCE_ARB: {
            typeStr = "Performance";
            break;
        }
        case GL_DEBUG_TYPE_OTHER_ARB: {
            typeStr = "Other";
            break;
        }
        default: {
            typeStr = "Unknown";
            break;
        }
    }
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH_ARB: {
            severityStr = "High";
            break;
        }
        case GL_DEBUG_SEVERITY_MEDIUM_ARB: {
            severityStr = "Medium";
            break;
        }
        case GL_DEBUG_SEVERITY_LOW_ARB: {
            severityStr = "Low";
            break;
        }
        default: {
            severityStr = "Unknown";
            break;
        }
    }
    if (type == GL_DEBUG_TYPE_ERROR_ARB) {
        HD_LOG_FATAL("OpenGL Debug:\n\tSource: {}\n\tType: {}\n\tId: {}\n\tSeverity: {}\n\tMessage: {}", sourceStr, typeStr, id, severityStr, message);
    }
    else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB || type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB || type == GL_DEBUG_TYPE_PORTABILITY_ARB || type == GL_DEBUG_TYPE_PERFORMANCE_ARB) {
        HD_LOG_WARNING("OpenGL Debug:\n\tSource: {}\n\tType: {}\n\tId: {}\n\tSeverity: {}\n\tMessage: {}", sourceStr, typeStr, id, severityStr, message);
    }
    else {
        HD_LOG_INFO("OpenGL Debug:\n\tSource: {}\n\tType: {}\n\tId: {}\n\tSeverity: {}\n\tMessage: {}", sourceStr, typeStr, id, severityStr, message);
    }
}

RenderSystem::RenderSystem() : impl(new Impl()) {
}

RenderSystem::~RenderSystem() {
    HD_DELETE(impl);
}

void RenderSystem::initialize() {
    glewExperimental = true;
    GLenum glewResult = glewInit();
    if (glewInit() != GLEW_OK) {
        HD_LOG_FATAL("Failed to initialize GLEW. Error: {}", glewGetErrorString(glewResult));
    }

    if (getEngine().getWindow().getOpenGLContextSettings().isDebug) {
        if (GLEW_ARB_debug_output) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
        }
        else {
            HD_LOG_WARNING("Failed to enable OpenGL debug output. Extension 'GLEW_ARB_debug_output' not supported");
        }
    }


    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vsCode = hd::FileStream("data/shaders/simpleVS.glsl", hd::FileMode::Read).readAllText();
    const char *vsSources[] = { vsCode.data() };
    const int vsLengths[] = { static_cast<int>(vsCode.size()) };
    glShaderSource(vertexShader, 1, vsSources, vsLengths);
    glCompileShader(vertexShader);

    uint32_t pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string psCode = hd::FileStream("data/shaders/simplePS.glsl", hd::FileMode::Read).readAllText();
    const char *psSources[] = { psCode.data() };
    const int psLengths[] = { static_cast<int>(psCode.size()) };
    glShaderSource(pixelShader, 1, psSources, psLengths);
    glCompileShader(pixelShader);

    impl->program = glCreateProgram();
    glAttachShader(impl->program, vertexShader);
    glAttachShader(impl->program, pixelShader);
    glLinkProgram(impl->program);
    glDeleteShader(vertexShader);
    glDeleteShader(pixelShader);

    
    glCreateVertexArrays(1, &impl->vertexFormat);

    glEnableVertexArrayAttrib(impl->vertexFormat, 0);
    glVertexArrayAttribBinding(impl->vertexFormat, 0, 0);
    glVertexArrayBindingDivisor(impl->vertexFormat, 0, 0);
    glVertexArrayAttribFormat(impl->vertexFormat, 0, 3, GL_FLOAT, false, 0);

    glEnableVertexArrayAttrib(impl->vertexFormat, 1);
    glVertexArrayAttribBinding(impl->vertexFormat, 1, 0);
    glVertexArrayBindingDivisor(impl->vertexFormat, 0, 0);
    glVertexArrayAttribFormat(impl->vertexFormat, 1, 2, GL_FLOAT, false, 12);


    const Vertex verts[] = {
        { -1.0f,  1.0f, 0.0f, 0.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f, 1.0f, 1.0f },
        { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },

        { -1.0f,  1.0f, 0.0f, 0.0f, 0.0f },
        {  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f, 1.0f, 1.0f },
    };
    glCreateBuffers(1, &impl->vertexBuffer);
    glNamedBufferData(impl->vertexBuffer, sizeof(verts), verts, GL_STATIC_DRAW);


    const Vertex vertsGUI[] = {
        { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },

        { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
    };
    glCreateBuffers(1, &impl->vertexBufferGUI);
    glNamedBufferData(impl->vertexBufferGUI, sizeof(vertsGUI), vertsGUI, GL_STATIC_DRAW);


    glCreateBuffers(1, &impl->constantBuffer);
    glNamedBufferData(impl->constantBuffer, sizeof(ConstantBuffer), nullptr, GL_DYNAMIC_DRAW);
}

void RenderSystem::shutdown() {
    for (auto &it : impl->createdTextures) {
        impl->destroyTexture(it);
    }

    glDeleteBuffers(1, &impl->constantBuffer);
    glDeleteBuffers(1, &impl->vertexBufferGUI);
    glDeleteBuffers(1, &impl->vertexBuffer);
    glDeleteBuffers(1, &impl->vertexFormat);
    glDeleteProgram(impl->program);
}

void RenderSystem::onEvent(const hd::WindowEvent &event) {
    if (event.type == hd::WindowEventType::Resize) {
        glViewport(0, 0, event.resize.width, event.resize.height);

        impl->proj = glm::perspectiveLH(glm::pi<float>() / 4.0f, static_cast<float>(event.resize.width) / event.resize.height, 0.1f, 1000.0f);
        impl->projGUI = hd::MathUtils::ortho2D(0, static_cast<float>(event.resize.width), static_cast<float>(event.resize.height), 0);
    }
}

void RenderSystem::onDraw() {
    const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(impl->vertexFormat);
    glUseProgram(impl->program);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, impl->constantBuffer);

    glBindVertexBuffer(0, impl->vertexBuffer, 0, sizeof(Vertex));
    impl->cbData.proj = impl->proj;
    for (const auto &rop : mRenderOps) {
        impl->cbData.view = glm::translate(-rop.camPos)*glm::rotate(-rop.camAngle, glm::vec3(0, 0, 1));
        impl->cbData.world = glm::translate(glm::vec3(rop.pos, 0))*glm::rotate(rop.angle, glm::vec3(0, 0, 1))*glm::scale(glm::vec3(rop.size, 1.0f));
        impl->cbData.uvOffset = rop.uvOffset;
        impl->cbData.uvSize = rop.uvSize;
        glNamedBufferSubData(impl->constantBuffer, 0, sizeof(ConstantBuffer), &impl->cbData);
        glBindTextureUnit(0, rop.texture->id);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    mRenderOps.clear();

    glBindVertexBuffer(0, impl->vertexBufferGUI, 0, sizeof(Vertex));
    impl->cbData.proj = impl->projGUI;
    impl->cbData.view = glm::mat4(1.0f);
    for (const auto &rop : mGUIRenderOps) {
        impl->cbData.world = glm::translate(glm::vec3(rop.pos, 0))*glm::rotate(rop.angle, glm::vec3(0, 0, 1))*glm::scale(glm::vec3(rop.size, 1.0f));
        impl->cbData.uvOffset = rop.uvOffset;
        impl->cbData.uvSize = rop.uvSize;
        glNamedBufferSubData(impl->constantBuffer, 0, sizeof(ConstantBuffer), &impl->cbData);
        glBindTextureUnit(0, rop.texture->id);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    mGUIRenderOps.clear();

    getEngine().getWindow().swapBuffers();
}

Texture *RenderSystem::createTexture(const void *data, uint32_t w, uint32_t h) {
    Texture *obj = new Texture();
    obj->size = glm::ivec2(w, h);

    glCreateTextures(GL_TEXTURE_2D, 1, &obj->id);
    glTextureStorage2D(obj->id, 1, GL_RGBA8, w, h);
    glTextureSubImage2D(obj->id, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(obj->id);

    impl->createdTextures.push_back(obj);
    return obj;
}

Texture* RenderSystem::createTexture(const hd::Image& img) {
    Texture *texture = createTexture(img.getData(), img.getSize().x, img.getSize().y);
    texture->path = hd::StringUtils::replace(img.getPath(), "data/textures/", "");
    return texture;
}

Texture *RenderSystem::createTextureFromFile(const std::string &path) {
    return createTexture(hd::Image("data/textures/" + path));
}

Texture *RenderSystem::createTextureFromColor(const hd::Color4 &color) {
    return createTexture(&color, 1, 1);
}

void RenderSystem::destroyTexture(Texture *&texture) {
    if (!texture) {
        HD_LOG_WARNING("Failed to destroy Texture. The texture is nullptr");
    }
    else {
        auto it = std::find(impl->createdTextures.begin(), impl->createdTextures.end(), texture);
        if (it != impl->createdTextures.end()) {
            impl->createdTextures.erase(std::remove(impl->createdTextures.begin(), impl->createdTextures.end(), texture), impl->createdTextures.end());
            impl->destroyTexture(texture);
        }
        else {
            HD_LOG_WARNING("Failed to destroy texture. The texture wasn't created by D3D11RenderSystem");
        }
    }
}

const glm::ivec2 &RenderSystem::getTextureSize(const Texture *texture) const {
    if (!texture) {
        HD_LOG_FATAL("texture is nullptr");
    }
    return texture->size;
}

const std::string &RenderSystem::getTexturePath(const Texture *texture) const {
    if (!texture) {
        HD_LOG_FATAL("texture is nullptr");
    }
    return texture->path;
}

void RenderSystem::addRenderOp(const RenderOp &rop, bool isGUI) {
    if (isGUI) {
        mGUIRenderOps.push_back(rop);
    }
    else {
        mRenderOps.push_back(rop);
    }
}

}

#endif
