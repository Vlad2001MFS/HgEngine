#include "Component.hpp"

namespace hg {

Component::Component() {
    mOwner = nullptr;
}

void Component::onSaveLoad(JSONObject &data, bool isLoad) {
}

void Component::onEvent(const SDL_Event &event) {
}

void Component::onFixedUpdate() {
}

void Component::onUpdate(float dt) {
}

Node *Component::getOwner() const {
    return mOwner;
}

}