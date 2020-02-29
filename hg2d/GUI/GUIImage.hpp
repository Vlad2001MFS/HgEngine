#pragma once
#include "GUIWidget.hpp"

namespace hg2d {

struct Texture;

class GUIImage : public GUIWidget {
    HG2D_OBJECT(GUIImage, GUIWidget);
public:
    GUIImage();

    virtual void onSaveLoad(JSONObject &data, bool isLoad) override;
    virtual void onDraw() override;

    void setTexture(const Texture *texture);
    const Texture *getTexture() const;

private:
    const Texture *mTexture;
};

}