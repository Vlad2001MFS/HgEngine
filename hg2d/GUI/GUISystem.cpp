#include "GUISystem.hpp"
#include "../Core/Engine.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "hd/Core/Log.hpp"
#include <algorithm>

namespace hg2d {

void GUISystem::initialize() {
    mFontSize = 16;
    mSkin.font = createFontFromFile(getEngine().getCreateInfo().gui.fontPath, mFontSize);
    mSkin.font->setHinting(FontHinting::Mono);
    mSkin.fontColor = getEngine().getCreateInfo().gui.fontColor;
    mSkin.buttonTexture = getRenderSystem().createTextureFromFile(getEngine().getCreateInfo().gui.buttonTexturePath);
    mSkin.hoveredButtonTexture = getRenderSystem().createTextureFromFile(getEngine().getCreateInfo().gui.hoveredButtonTexturePath);
    mSkin.clickedButtonTexture = getRenderSystem().createTextureFromFile(getEngine().getCreateInfo().gui.clickedButtonTexturePath);
    mSkin.alignSpaceX = getEngine().getCreateInfo().gui.alignSpaceX;
    mSkin.alignSpaceY = getEngine().getCreateInfo().gui.alignSpaceY;
}

void GUISystem::shutdown() {
    getRenderSystem().destroyTexture(mSkin.clickedButtonTexture);
    getRenderSystem().destroyTexture(mSkin.hoveredButtonTexture);
    getRenderSystem().destroyTexture(mSkin.buttonTexture);
    for (auto &it : mCreatedFonts) {
        HD_DELETE(it);
    }
}

void GUISystem::onEvent(const hd::WindowEvent &event) {
    if (event.type == hd::WindowEventType::Resize) {
        constexpr float ideal = 16.0f / (640.0f + 480.0f);
        int size = ideal*(event.resize.width + event.resize.height);
        destroyFont(mSkin.font);
        mSkin.font = createFontFromFile(getEngine().getCreateInfo().gui.fontPath, size);
        mSkin.font->setHinting(FontHinting::Mono);
        HD_LOG_INFO("Font resized from '{}' to '{}'", mFontSize, size);
        mFontSize = size;
    }
}

Font *GUISystem::createFontFromFile(const std::string &filename, uint32_t size) {
    if (!filename.empty() && size > 0) {
        std::string path = "data/configs/" + filename;
        Font *font = new Font(path, size);
        mCreatedFonts.push_back(font);
        return font;
    }
    else {
        HD_LOG_FATAL("Failed to create font from file '{}' with size '{}'", filename.data(), size);
        return nullptr;
    }
}

void GUISystem::destroyFont(Font *&font) {
    if (!font) {
        HD_LOG_WARNING("font is nullptr");
    }
    else {
        auto it = std::find(mCreatedFonts.begin(), mCreatedFonts.end(), font);
        if (it != mCreatedFonts.end()) {
            mCreatedFonts.erase(std::remove(mCreatedFonts.begin(), mCreatedFonts.end(), font), mCreatedFonts.end());
            HD_DELETE(font);
        }
        else {
            HD_LOG_WARNING("Failed to destroy Font. The Font wasn't created by GUISystem");
        }
    }
}

}
