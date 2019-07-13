#pragma once
#include "AGUIWidget.hpp"

namespace hg2d {

struct Texture;

class GUIImage : public AGUIWidget {
public:
    explicit GUIImage(Engine &engine);
    ~GUIImage();

    void setTexture(const Texture *texture);
    const Texture *getTexture() const;

    void onDraw();

private:
    const Texture *mTexture;
};

}