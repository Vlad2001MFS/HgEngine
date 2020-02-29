#pragma once
#include "GUIWidget.hpp"

namespace hg2d {

struct Texture;

class GUIImage : public GUIWidget {
    HG2D_OBJECT(GUIImage, GUIWidget);
public:
    GUIImage();

    void setTexture(const Texture *texture);
    const Texture *getTexture() const;

    virtual void onDraw() override;

private:
    const Texture *mTexture;
};

}