#include "RenderSystem.hpp"
#include "../Core/BuildConfig.hpp"

#ifdef HG2D_RENDERER_D3D11

#include "../Core/Engine.hpp"
#include "hd/Math/hdMathUtils.hpp"
#include "hd/IO/hdImage.hpp"
#include "3rd/include/glm/ext.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <algorithm>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_RELEASE(obj) do { if (obj) { obj->Release(); obj = nullptr; } } while (false)

namespace hg2d {

struct Texture {
    ID3D11Texture2D *tex;
    ID3D11ShaderResourceView *srv;
    glm::ivec2 size;
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
        this->device = nullptr;
        this->swapChain = nullptr;
        this->context = nullptr;
        this->defaultRTV = nullptr;
    }

    void destroyTexture(Texture *&obj) {
        SAFE_RELEASE(obj->srv);
        SAFE_RELEASE(obj->tex);
        HD_DELETE(obj);
    }

    ID3D11Device *device;
    IDXGISwapChain *swapChain;
    ID3D11DeviceContext *context;
    ID3D11RenderTargetView *defaultRTV;

    ID3D11VertexShader *vertexShader;
    ID3D11PixelShader *pixelShader;
    ID3D11InputLayout *vertexFormat;
    ID3D11Buffer *vertexBuffer;
    ID3D11Buffer *vertexBufferGUI;
    ID3D11Buffer *constantBuffer;
    ID3D11BlendState *blendState;

    std::vector<Texture*> createdTextures;
    ConstantBuffer cbData;
    glm::mat4 proj, projGUI;
};

RenderSystem::RenderSystem(Engine &engine) : mEngine(engine), impl(std::make_unique<Impl>()) {
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::onInitialize() {
    if (mEngine.getWindow().hasOpenGLContext()) {
        HD_LOG_FATAL("Failed to initialize D3D11RenderSystem. Engine window was created with OpenGL context");
    }

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.Width = mEngine.getWindow().getSizeX();
    sd.BufferDesc.Height = mEngine.getWindow().getSizeY();
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.Flags = 0;
    sd.OutputWindow = static_cast<HWND>(mEngine.getWindow().getOSWindowHandle());
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Windowed = !HD_FLAG_EXIST(mEngine.getWindow().getFlags(), hd::WindowFlags::Fullscreen);

#ifdef HD_BUILDMODE_DEBUG
    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, &sd, &impl->swapChain, &impl->device, nullptr, &impl->context);
#else
    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &impl->swapChain, &impl->device, nullptr, &impl->context);
#endif

    ID3D11Texture2D *backBuffer = nullptr;
    impl->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    impl->device->CreateRenderTargetView(backBuffer, nullptr, &impl->defaultRTV);
    backBuffer->Release();

    impl->context->OMSetRenderTargets(1, &impl->defaultRTV, nullptr);

    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.MaxDepth = 1.0f;
    vp.MinDepth = 0.0f;
    vp.Width = mEngine.getWindow().getSizeX();
    vp.Height = mEngine.getWindow().getSizeY();
    impl->context->RSSetViewports(1, &vp);

    uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef HD_BUILDMODE_DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif
    ID3DBlob *vsBlob = nullptr;
    D3DCompileFromFile(L"data/shaders/simpleVS.hlsl", nullptr, nullptr, "main", "vs_5_0", flags, 0, &vsBlob, nullptr);
    impl->device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &impl->vertexShader);

    D3D11_INPUT_ELEMENT_DESC vertexFormat[] = {
        { "ATTRIB", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "ATTRIB", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    impl->device->CreateInputLayout(vertexFormat, HD_ARRAYSIZE(vertexFormat), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &impl->vertexFormat);
    vsBlob->Release();

    ID3DBlob *psBlob = nullptr;
    D3DCompileFromFile(L"data/shaders/simplePS.hlsl", nullptr, nullptr, "main", "ps_5_0", flags, 0, &psBlob, nullptr);
    impl->device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &impl->pixelShader);
    psBlob->Release();

    const Vertex verts[] = {
        { -0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
    {  0.5f, -0.5f, 0.0f, 1.0f, 1.0f },
    { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f },

    { -0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
    {  0.5f,  0.5f, 0.0f, 1.0f, 0.0f },
    {  0.5f, -0.5f, 0.0f, 1.0f, 1.0f },
    };
    D3D11_BUFFER_DESC vbDesc;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.ByteWidth = sizeof(verts);
    vbDesc.CPUAccessFlags = 0;
    vbDesc.MiscFlags = 0;
    vbDesc.StructureByteStride = 0;
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    D3D11_SUBRESOURCE_DATA vbData;
    vbData.pSysMem = verts;
    vbData.SysMemPitch = 0;
    vbData.SysMemSlicePitch = 0;
    impl->device->CreateBuffer(&vbDesc, &vbData, &impl->vertexBuffer);

    const Vertex vertsGUI[] = {
        { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },

    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
    };
    D3D11_BUFFER_DESC vbDescGUI;
    vbDescGUI.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDescGUI.ByteWidth = sizeof(vertsGUI);
    vbDescGUI.CPUAccessFlags = 0;
    vbDescGUI.MiscFlags = 0;
    vbDescGUI.StructureByteStride = 0;
    vbDescGUI.Usage = D3D11_USAGE_DEFAULT;
    D3D11_SUBRESOURCE_DATA vbDataGUI;
    vbDataGUI.pSysMem = vertsGUI;
    vbDataGUI.SysMemPitch = 0;
    vbDataGUI.SysMemSlicePitch = 0;
    impl->device->CreateBuffer(&vbDescGUI, &vbDataGUI, &impl->vertexBufferGUI);

    D3D11_BUFFER_DESC cbDesc;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.ByteWidth = sizeof(ConstantBuffer);
    cbDesc.CPUAccessFlags = 0;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    impl->device->CreateBuffer(&cbDesc, nullptr, &impl->constantBuffer);

    D3D11_BLEND_DESC blendDesc;
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    impl->device->CreateBlendState(&blendDesc, &impl->blendState);
}

void RenderSystem::onShutdown() {
    for (auto &it : impl->createdTextures) {
        impl->destroyTexture(it);
    }

    SAFE_RELEASE(impl->blendState);
    SAFE_RELEASE(impl->constantBuffer);
    SAFE_RELEASE(impl->vertexBufferGUI);
    SAFE_RELEASE(impl->vertexBuffer);
    SAFE_RELEASE(impl->pixelShader);
    SAFE_RELEASE(impl->vertexFormat);
    SAFE_RELEASE(impl->vertexShader);

    SAFE_RELEASE(impl->defaultRTV);
    SAFE_RELEASE(impl->context);
    SAFE_RELEASE(impl->swapChain);
    SAFE_RELEASE(impl->device);
}

void RenderSystem::onEvent(const hd::WindowEvent &event) {
    if (event.type == hd::WindowEventType::Resize) {
        D3D11_VIEWPORT vp;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        vp.MaxDepth = 1.0f;
        vp.MinDepth = 0.0f;
        vp.Width = event.resize.width;
        vp.Height = event.resize.height;
        impl->context->RSSetViewports(1, &vp);

        impl->proj = glm::perspectiveLH(glm::pi<float>() / 4.0f, static_cast<float>(event.resize.width) / event.resize.height, 0.1f, 1000.0f);
        impl->projGUI = hd::MathUtils::ortho2D(0, event.resize.width, event.resize.height, 0);
    }
}

void RenderSystem::onDraw() {
    const float clearColor[] = { 0.5f, 0.5f, 1.0f, 1.0f };
    impl->context->ClearRenderTargetView(impl->defaultRTV, clearColor);

    uint32_t stride = sizeof(Vertex);
    uint32_t offset = 0;
    const float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    impl->context->OMSetBlendState(impl->blendState, blendFactor, 0xFFFFFFFF);
    impl->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    impl->context->IASetInputLayout(impl->vertexFormat);
    impl->context->VSSetShader(impl->vertexShader, nullptr, 0);
    impl->context->PSSetShader(impl->pixelShader, nullptr, 0);
    impl->context->VSSetConstantBuffers(0, 1, &impl->constantBuffer);
    impl->cbData.proj = glm::transpose(impl->proj);
    impl->cbData.view = glm::transpose(glm::lookAtLH(glm::vec3(0, 0, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
    impl->context->IASetVertexBuffers(0, 1, &impl->vertexBuffer, &stride, &offset);
    for (const auto &rop : mRenderOps) {
        impl->cbData.world = glm::transpose(glm::translate(glm::vec3(rop.pos, 0))*glm::rotate(rop.angle, glm::vec3(0, 0, 1))*glm::scale(glm::vec3(rop.size, 1.0f)));
        impl->cbData.uvOffset = rop.uvOffset;
        impl->cbData.uvSize = rop.uvSize;
        impl->context->UpdateSubresource(impl->constantBuffer, 0, nullptr, &impl->cbData, 0, 0);
        impl->context->PSSetShaderResources(0, 1, &rop.texture->srv);
        impl->context->Draw(6, 0);
    }
    mRenderOps.clear();

    impl->cbData.proj = glm::transpose(impl->projGUI);
    impl->cbData.view = glm::transpose(glm::mat4(1.0f));
    impl->context->IASetVertexBuffers(0, 1, &impl->vertexBufferGUI, &stride, &offset);
    for (const auto &rop : mGUIRenderOps) {
        impl->cbData.world = glm::transpose(glm::translate(glm::vec3(rop.pos, 0))*glm::rotate(rop.angle, glm::vec3(0, 0, 1))*glm::scale(glm::vec3(rop.size, 1.0f)));
        impl->cbData.uvOffset = rop.uvOffset;
        impl->cbData.uvSize = rop.uvSize;
        impl->context->UpdateSubresource(impl->constantBuffer, 0, nullptr, &impl->cbData, 0, 0);
        impl->context->PSSetShaderResources(0, 1, &rop.texture->srv);
        impl->context->Draw(6, 0);
    }
    mGUIRenderOps.clear();

    impl->swapChain->Present(0, 0);
}

Texture *RenderSystem::createTexture(const void *data, uint32_t w, uint32_t h) {
    Texture *obj = new Texture();
    obj->size = glm::ivec2(w, h);

    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.ArraySize = 1;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    texDesc.Width = w;
    texDesc.Height = h;
    texDesc.MipLevels = 1;
    texDesc.MiscFlags = 0;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    if (data) {
        D3D11_SUBRESOURCE_DATA texData;
        texData.pSysMem = data;
        texData.SysMemPitch = sizeof(uint32_t)*w;
        texData.SysMemSlicePitch = 0;
        impl->device->CreateTexture2D(&texDesc, &texData, &obj->tex);
    }
    else {
        impl->device->CreateTexture2D(&texDesc, nullptr, &obj->tex);
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Format = texDesc.Format;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    impl->device->CreateShaderResourceView(obj->tex, &srvDesc, &obj->srv);

    impl->createdTextures.push_back(obj);
    return obj;
}

Texture* RenderSystem::createTexture(const hd::Image& img) {
    return createTexture(img.getPixels(), img.getWidth(), img.getHeight());
}

Texture *RenderSystem::createTextureFromFile(const std::string &path) {
    return createTexture(hd::Image("data/textures/" + path));
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

void RenderSystem::addRenderOp(const RenderOp &rop) {
    if (rop.isGUI) {
        mGUIRenderOps.push_back(rop);
    }
    else {
        mRenderOps.push_back(rop);
    }
}

}

#endif
