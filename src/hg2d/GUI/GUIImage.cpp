#include "GUIImage.hpp"
#include "../Renderer/RenderSystem.hpp"

namespace hg2d {

GUIImage::GUIImage(Engine &engine) : AGUIWidget(engine) {
    mTexture = nullptr;
}

void GUIImage::setTexture(const Texture *texture) {
    mTexture = texture;
    if (getSize().x == 0 && getSize().y == 0) {
        setSize(mRenderSystem.getTextureSize(mTexture));
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
        rop.isGUI = true;
        mRenderSystem.addRenderOp(rop);
    }
}
    
}
