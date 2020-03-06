#include "GUIImage.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../Cache/CacheSystem.hpp"

namespace hg {

HG_REGISTER_OBJECT(GUIImage);

GUIImage::GUIImage() {
    mTexture = nullptr;
}

void GUIImage::onSaveLoad(JSONObject &data, bool isLoad) {
    if (isLoad) {
        mTexture = getCacheSystem().loadTexture(data["texture"].get<std::string>());
    }
    else {
        data["texture"] = getRenderSystem().getTexturePath(mTexture);
    }

    BaseClassName::onSaveLoad(data, isLoad);
}

void GUIImage::onDraw() {
    if (mTexture) {
        RenderOp rop;
        rop.texture = mTexture;
        rop.pos = getAbsolutePosition();
        rop.size = getAbsoluteSize();
        getRenderSystem().addRenderOp(rop, true);
    }

    BaseClassName::onDraw();
}

void GUIImage::setTexture(const Texture *texture) {
    mTexture = texture;
}

const Texture *GUIImage::getTexture() const {
    return mTexture;
}
    
}
