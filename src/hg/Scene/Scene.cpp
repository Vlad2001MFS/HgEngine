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
    mOnUpdate(dt);
}

void Scene::clear() {
    destroyAllComponents();
    destroyAllChildren();
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

Scene &getScene() {
    return getEngine().getScene();
}

}
