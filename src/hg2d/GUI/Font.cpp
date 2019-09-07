#include "Font.hpp"
#include "..//Renderer/RenderSystem.hpp"
#include "hd/Core/hdStringUtils.hpp"
#include "hd/Core/hdLog.hpp"

namespace hg2d {

Font::Font(Engine &engine, const std::string &filename, uint32_t size) : AEngineObject(engine), mFont(filename, size) {
}

Texture *Font::renderLine(const std::string &text, const hd::Color4 &color) const {
    if (text.empty()) {
        HD_LOG_WARNING("Rendering empty text line to texture");
    }
    if (color.a == 0) {
        HD_LOG_WARNING("Rendering fully transparent text line to texture");
    }
    hd::Image img(nullptr, glm::ivec2(mFont.calcTextWidth(text), mFont.getLineHeight()), hd::ImageFormat::RGBA);
    mRenderLine(img, 0, 0, text, color);
    return mRenderSystem.createTexture(img);
}

Texture *Font::renderText(const std::string &text, const hd::Color4 &color) const {
    if (text.empty()) {
        HD_LOG_WARNING("Rendering empty text to texture");
    }
    if (color.a == 0) {
        HD_LOG_WARNING("Rendering fully transparent text to texture");
    }
    std::vector<std::string> lines = hd::StringUtils::split(text, "\n", false);
    auto maxLine = std::max_element(lines.begin(), lines.end(), [&](const std::string &a, const std::string &b){
        return mFont.calcTextWidth(a) < mFont.calcTextWidth(b);
    });
    size_t width =  mFont.calcTextWidth(*maxLine);
    size_t height = lines.size()*mFont.getLineHeight();

    hd::Image img(nullptr, glm::ivec2(width, height), hd::ImageFormat::RGBA);
    for (uint32_t i = 0; i < lines.size(); i++) {
        mRenderLine(img, 0, i*mFont.getLineHeight(), lines[i], color);
    }
    return mRenderSystem.createTexture(img);
}

void Font::setStyle(FontStyle style) {
    mFont.setStyle(style);
}

void Font::setOutline(uint32_t outline) {
    mFont.setOutline(outline);
}

void Font::setHinting(FontHinting hinting) {
    mFont.setHinting(hinting);
}

void Font::setKerning(bool kerning) {
    mFont.setKerning(kerning);
}

uint32_t Font::getMaxGlyphHeight() const {
    return mFont.getMaxGlyphHeight();
}

uint32_t Font::getLineHeight() const {
    return mFont.getLineHeight();
}

void Font::mRenderLine(hd::Image &target, uint32_t uOffset, uint32_t vOffset, const std::string &text, const hd::Color4 &color) const {
    hd::Image img = mFont.renderLine(text, color);
    for (int y = 0; y < img.getSize().y; y++) {
        const hd::Color4 *line = static_cast<const hd::Color4*>(img.getData()) + y*img.getSize().x;
        int baseOffset = uOffset + vOffset*target.getSize().x;
        int offset = baseOffset + y*target.getSize().x;
        int size = img.getSize().x;
        memcpy(static_cast<hd::Color4*>(target.getData()) + static_cast<size_t>(offset), line, sizeof(hd::Color4)*static_cast<size_t>(size));
    }
}

}
