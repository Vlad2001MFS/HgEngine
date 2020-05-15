#include "Component.hpp"

namespace hg {

void Component::onSaveLoad(hd::JSON &data, bool isLoad) {
}

bool Component::onInitialize() {
    return true;
}

void Component::onEvent(const WindowEvent &event) {
}

void Component::onFixedUpdate() {
}

void Component::onUpdate(float dt) {
}

GameObject *Component::getOwner() const {
    return mOwner;
}

}
