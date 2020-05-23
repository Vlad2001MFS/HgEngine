#pragma once
#include "Component.hpp"
#include "../Graphics/Texture2D.hpp"

namespace hg {

class Sprite : public Component {
    HG_OBJECT(Sprite, Component);
public:
    void onSaveLoad(hd::JSON &data, bool isLoad) override;
    void onUpdate(float dt) override;

    void setTexture(const std::string &path);
    void setLayer(int layer);

    const std::string &getTexture() const;
    int getLayer() const;

private:
    Texture2DPtr mTexture;
    std::string mTexturePath;
    int mLayer = 0;
};

}
