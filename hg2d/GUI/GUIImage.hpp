#pragma once
#include "GUIWidget.hpp"

namespace hg2d {

struct Texture;

class GUIImage : public GUIWidget {
public:
    GUIImage();

    void setTexture(const Texture *texture);
    const Texture *getTexture() const;

    virtual void onDraw() override;

private:
    const Texture *mTexture;
};

HG2D_REGISTER_OBJECT("GUIImage", GUIImage);

}