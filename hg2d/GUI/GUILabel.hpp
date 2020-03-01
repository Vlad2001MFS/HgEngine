#pragma once
#include "GUIWidget.hpp"
#include "hd/Core/Color.hpp"

namespace hg2d {

struct Texture;

class GUILabel : public GUIWidget {
    HG2D_OBJECT(GUILabel, GUIWidget);
public:
    GUILabel();
    ~GUILabel();

    virtual void onSaveLoad(JSONObject &data, bool isLoad) override;
    virtual void onEvent(const hd::WindowEvent &event) override;
    virtual void onDraw() override;

    void setText(const std::string &text, bool updateSize);
    const std::string &getText() const;
    void setColor(const hd::Color4 &color);
    const hd::Color4 &getColor() const;

private:
    void mUpdateTexture();

    Texture *mTexture;
    std::string mText;
    hd::Color4 mColor;
};

}
