#pragma once
#include "GUIWidget.hpp"
#include "../Graphics/Texture2D.hpp"

namespace hg {

class GUIImage : public GUIWidget {
public:
    void onUpdate(float dt) override;

    void setTexture(const Texture2DPtr &texture);
    const Texture2DPtr &getTexture() const;

private:
    Texture2DPtr mTexture;
};

}
