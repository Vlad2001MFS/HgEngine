#include "GUIImage.hpp"
#include "../Renderer2D/RenderSystem2D.hpp"

namespace hg {

void GUIImage::onSaveLoad(hd::JSON &data, bool isLoad) {
    if (isLoad) {
        mTexture = getRenderDevice().loadTexture2D(data["texture"].get<std::string>());
    }
    else {
        data["texture"] = mTexture->getPath();
    }

    BaseClassName::onSaveLoad(data, isLoad);
}

void GUIImage::onUpdate(float dt) {
    if (mTexture) {
        getRenderSystem2D().drawTextureGUI(mTexture, getAbsolutePosition(), getSize());
    }

    BaseClassName::onUpdate(dt);
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
