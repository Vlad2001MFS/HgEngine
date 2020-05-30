#include "Scene.hpp"
#include "Camera.hpp"
#include "../Core/Engine.hpp"
#include "../Renderer2D/RenderSystem2D.hpp"
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

    if (mCamera) {
        getRenderSystem2D().setCamera(mCamera->getOwner()->getWorldPosition(), mCamera->getOwner()->getWorldAngle(), mCamera->getDistance());
    }
    else {
        getRenderSystem2D().setCamera(glm::vec2(0, 0), 0.0f, 1.0f);
    }
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

void Scene::setCameraObject(GameObject *go) {
    if (go) {
        Camera *camera = go->findComponent<Camera>();
        if (camera) {
            mCamera = camera;
        }
        else {
            HD_LOG_WARNING("Failed to set GameObject without Camera component as scene Camera");
        }
    }
    else {
        HD_LOG_WARNING("Failed to set null GameObject as Camera");
    }
}

std::string Scene::mGetFullPath(const std::string &path) {
    return "./data/levels/" + path;
}

void Scene::mOnCreateComponent(Component *component) {
    mComponentsForFirstUpdate.push_back(component);
}

void Scene::mOnDestroyComponent(Component *component) {
    if (mCamera == component) {
        mCamera = nullptr;
        HD_LOG_INFO("Active camera component was destroyed");
    }
}

Scene &getScene() {
    return getEngine().getScene();
}

}
