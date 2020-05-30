#include "Scene.hpp"
#include "../Core/Engine.hpp"
#include "hd/IO/FileStream.hpp"

namespace hg {

void Scene::onEvent(const WindowEvent &event) {
    mOnEvent(event);
}

void Scene::onFixedUpdate() {
    mOnFixedUpdate();
}

void Scene::onUpdate(float dt) {
    for (auto &component : mComponentsForFirstUpdate) {
        component->onFirstUpdate();
    }
    mComponentsForFirstUpdate.clear();

    mOnUpdate(dt);
}

void Scene::clear() {
    destroyAllChildren();
    destroyAllComponents();
}

void Scene::save(const std::string &path) {
    hd::JSON data;
    mOnSaveLoad(data, false);
    std::string text = data.dump(2);

    hd::FileStream file = hd::FileStream(mGetFullPath(path), hd::FileMode::Write);
    file.writeLine(text);
}

void Scene::load(const std::string &path) {
    clear();

    hd::FileStream file = hd::FileStream(mGetFullPath(path), hd::FileMode::Read);
    std::string text = file.readAllText();

    hd::JSON data = hd::JSON::parse(text);
    mOnSaveLoad(data, true);
}

std::string Scene::mGetFullPath(const std::string &path) {
    return "./data/levels/" + path;
}

void Scene::mOnCreateComponent(Component *component) {
    mComponentsForFirstUpdate.push_back(component);
}

Scene &getScene() {
    return getEngine().getScene();
}

}
