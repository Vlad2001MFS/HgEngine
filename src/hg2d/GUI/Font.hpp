#pragma once
#include "../Core/AEngineObject.hpp"
#include "hd/IO/hdFont.hpp"

namespace hg2d {

using hd::FontStyle;
using hd::FontHinting;

struct Texture;

class Font : public AEngineObject {
public:
    Font(Engine &engine, const std::string &filename, uint32_t size);
    ~Font();
    
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
