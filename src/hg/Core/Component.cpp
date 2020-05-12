#include "Component.hpp"

namespace hg {

Component::Component() {
    mOwner = nullptr;
}

void Component::onSaveLoad(hd::JSON &data, bool isLoad) {
}

void Component::onEvent(const WindowEvent &event) {
}

void Component::onFixedUpdate() {
}

void Component::onUpdate(float dt) {
}

Node *Component::getOwner() const {
    return mOwner;
}

}
