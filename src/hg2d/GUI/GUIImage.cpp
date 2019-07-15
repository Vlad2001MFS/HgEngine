#include "GUIImage.hpp"
#include "../Core/Engine.hpp"

namespace hg2d {

GUIImage::GUIImage(Engine &engine) : AGUIWidget(engine) {
    mTexture = nullptr;
}

GUIImage::~GUIImage() {
}

void GUIImage::setTexture(const Texture *texture) {
    mTexture = texture;
    if (getSize().x == 0 && getSize().y == 0) {
        setSize(mEngine.getRenderSystem().getTextureSize(mTexture));
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
        mEngine.getRenderSystem().addRenderOp(rop);
    }
}
    
}
