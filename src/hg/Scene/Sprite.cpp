#include "Sprite.hpp"
#include "GameObject.hpp"
#include "../Renderer2D/RenderSystem2D.hpp"

namespace hg {

void Sprite::onSaveLoad(hd::JSON &data, bool isLoad) {
    if (isLoad) {
        setTexture(data["texture"]);
        setLayer(data["layer"]);
    }
    else {
        data["texture"] = mTexturePath;
        data["layer"] = mLayer;
    }
}

void Sprite::onUpdate(float dt) {
    if (mTexture) {
        glm::vec3 pos = glm::vec3(getOwner()->getWorldPosition(), mLayer);
        getRenderSystem2D().drawTexture(mTexture, pos, getOwner()->getSize(), getOwner()->getWorldAngle());
    }
}

void Sprite::setTexture(const std::string &path) {
    if (!path.empty()) {
        mTexture = hg::getRenderDevice().loadTexture2D(path);
    }
    else {
        mTexture.reset();
    }
    mTexturePath = path;
}

void Sprite::setLayer(int layer) {
    mLayer = layer;
}

const std::string &Sprite::getTexture() const {
    return mTexturePath;
}

int Sprite::getLayer() const {
    return mLayer;
}

}
