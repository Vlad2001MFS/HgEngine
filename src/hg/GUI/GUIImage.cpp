#include "GUIImage.hpp"
#include "../Renderer2D/RenderSystem2D.hpp"

namespace hg {

void GUIImage::onUpdate(float dt) {
    if (mTexture) {
        getRenderSystem2D().drawTextureGUI(mTexture, getAbsolutePosition(), getSize());
    }
}

void GUIImage::setTexture(const Texture2DPtr &texture) {
    mTexture = texture;
    if (getSize().x == 0 && getSize().y == 0) {
        setSize(mTexture->getSize());
    }
}

const Texture2DPtr &GUIImage::getTexture() const {
    return mTexture;
}

}
