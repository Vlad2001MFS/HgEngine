#pragma once
#include "AGUIWidget.hpp"

namespace hg2d {

struct Texture;

class GUIImage : public AGUIWidget {
public:
    GUIImage();
    virtual ~GUIImage() = default;

    void setTexture(const Texture *texture);
    const Texture *getTexture() const;

    virtual void onDraw() override;

private:
    const Texture *mTexture;
};

}