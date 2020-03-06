#pragma once
#include "GUIWidget.hpp"

namespace hg {

struct Texture;

class GUIImage : public GUIWidget {
    HG_OBJECT(GUIImage, GUIWidget);
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
