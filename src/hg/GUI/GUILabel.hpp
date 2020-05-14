#pragma once
#include "GUIWidget.hpp"
#include "../Graphics/Texture2D.hpp"

namespace hg {

class GUILabel : public GUIWidget {
public:
    GUILabel();

    void onUpdate(float dt) override;

    void setText(const std::string &text, bool updateSize);
    const std::string &getText() const;
    void setColor(const glm::vec4 &color);
    const glm::vec4 &getColor() const;

private:
    void mUpdateTexture();

    Texture2DPtr mTexture;
    std::string mText;
    glm::vec4 mColor;
};

}
