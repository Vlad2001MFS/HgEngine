#include "GUIImage.hpp"
#include "../Renderer/RenderSystem.hpp"

namespace hg2d {

HG2D_REGISTER_OBJECT(GUIImage);

GUIImage::GUIImage() {
    mTexture = nullptr;
}

void GUIImage::setTexture(const Texture *texture) {
    mTexture = texture;
    if (getSize().x == 0 && getSize().y == 0) {
        setSize(getRenderSystem().getTextureSize(mTexture));
    }
}

const Texture *GUIImage::getTexture() const {
    return mTexture;
}
void GUIImage::onDraw() {
    if (mTexture) {
        RenderOp rop;
        rop.texture = mTexture;
        rop.pos = getAbsolutePosition();
        rop.size = getSize();
        getRenderSystem().addRenderOp(rop, true);
    }

    GUIWidget::onDraw();
}
    
}
