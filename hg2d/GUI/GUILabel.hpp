#pragma once
#include "GUIWidget.hpp"
#include "hd/Core/Color.hpp"

namespace hg2d {

struct Texture;

class GUILabel : public GUIWidget {
public:
    GUILabel();
    virtual ~GUILabel();

    void setText(const std::string &text, bool updateSize);
    const std::string &getText() const;
    void setColor(const hd::Color4 &color);
    const hd::Color4 &getColor() const;

    virtual void onDraw() override;

private:
    void mUpdateTexture();

    Texture *mTexture;
    std::string mText;
    hd::Color4 mColor;
};

HG2D_REGISTER_OBJECT("GUILabel", GUILabel);

}
