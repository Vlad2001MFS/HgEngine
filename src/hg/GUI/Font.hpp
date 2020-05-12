#pragma once
#include "hd/IO/Image.hpp"
#include <SDL2/SDL_ttf.h>
#include <memory>

namespace hg {

enum class FontStyle {
    Normal = TTF_STYLE_NORMAL,
    Bold = TTF_STYLE_BOLD,
    Italic = TTF_STYLE_ITALIC,
    Underline = TTF_STYLE_UNDERLINE,
    Strikethrough = TTF_STYLE_STRIKETHROUGH
};

enum class FontHinting {
    Normal = TTF_HINTING_NORMAL,
    Light = TTF_HINTING_LIGHT,
    Mono = TTF_HINTING_MONO,
    None = TTF_HINTING_NONE
};

using FontPtr = std::shared_ptr<class Font>;

class Font {
public:
    explicit Font(TTF_Font *font);
    ~Font();

    hd::Image renderLine(const std::string &text, const glm::vec4 &color) const;
    hd::Image renderText(const std::string &text, const glm::vec4 &color) const;

    void setStyle(FontStyle style);
    void setOutline(uint32_t outline);
    void setHinting(FontHinting hinting);
    void setKerning(bool kerning);
    uint32_t getMaxGlyphHeight() const;
    uint32_t getLineHeight() const;
    uint32_t getTextWidth(const std::string &text) const;

    static FontPtr createFromFile(const std::string &path, uint32_t size);

private:
    hd::Image mRenderLine(const std::string &text, const glm::vec4 &color) const;
    void mRenderLineTo(hd::Image &target, uint32_t uOffset, uint32_t vOffset, const std::string &text, const glm::vec4 &color) const;

    static std::string mGetFullPath(const std::string &path);

    TTF_Font *mFont = nullptr;
};

}
