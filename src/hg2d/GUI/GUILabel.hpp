#pragma once
#include "AGUIWidget.hpp"
#include "hd/Core/hdColor.hpp"

namespace hg2d {

struct Texture;

class GUILabel : public AGUIWidget {
public:
    explicit GUILabel(Engine &engine);
    virtual ~GUILabel();

    void setText(const std::string &text);
    const std::string &getText() const;
    void setColor(const hd::Color4 &color);
    const hd::Color4 &getColor() const;

    void onDraw();

private:
    void mUpdateTexture();

    Texture *mTexture;
    std::string mText;
    hd::Color4 mColor;
};

}
