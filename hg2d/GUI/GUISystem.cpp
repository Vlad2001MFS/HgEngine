#include "GUISystem.hpp"
#include "../Core/Engine.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "hd/Core/Log.hpp"
#include <algorithm>

namespace hg2d {

void GUISystem::initialize() {
    mSkin.font = createFontFromFile(getEngine().getCreateInfo().gui.fontPath, getEngine().getCreateInfo().gui.fontSize);
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

Font *GUISystem::createFontFromFile(const std::string &filename, uint32_t size) {
    if (!filename.empty() && size > 0) {
        std::string path = "data/configs/" + filename;
        Font *font = new Font(path, size);
        mCreatedFonts.push_back(font);
        return font;
    }
    else {
        LOG_F(FATAL, "Failed to create font from file '{}' with size '{}'", filename.data(), size);
        return nullptr;
    }
}

void GUISystem::destroyFont(Font *&font) {
    if (!font) {
        LOG_F(WARNING, "font is nullptr");
    }
    else {
        auto it = std::find(mCreatedFonts.begin(), mCreatedFonts.end(), font);
        if (it != mCreatedFonts.end()) {
            HD_DELETE(font);
            mCreatedFonts.erase(std::remove(mCreatedFonts.begin(), mCreatedFonts.end(), font), mCreatedFonts.end());
        }
        else {
            LOG_F(WARNING, "Failed to destroy Font. The Font wasn't created by GUISystem");
        }
    }
}

}
