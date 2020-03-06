#pragma once
#include "hd/IO/Font.hpp"

namespace hg {

using hd::FontStyle;
using hd::FontHinting;

struct Texture;

class Font final {
public:
    Font(const std::string &filename, uint32_t size);
    
    Texture *renderLine(const std::string &text, const hd::Color4 &color) const;
    Texture *renderText(const std::string &text, const hd::Color4 &color) const;

    void setStyle(FontStyle style);
    void setOutline(uint32_t outline);
    void setHinting(FontHinting hinting);
    void setKerning(bool kerning);
    uint32_t getMaxGlyphHeight() const;
    uint32_t getLineHeight() const;

private:
    void mRenderLine(hd::Image &target, uint32_t uOffset, uint32_t vOffset, const std::string &text, const hd::Color4 &color) const;

    hd::Font mFont;
};
    
}
