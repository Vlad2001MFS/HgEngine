#pragma once
#include "GameObject.hpp"

namespace hg {

class Camera;

class Scene : public GameObject {
    friend class GameObject;
public:
    void onEvent(const WindowEvent &event);
    void onFixedUpdate();
    void onUpdate(float dt);

    void clear();
    void save(const std::string &path);
    void load(const std::string &path);

    void setCameraObject(GameObject *go);

private:
    static std::string mGetFullPath(const std::string &path);

    void mOnCreateComponent(Component *component);
    void mOnDestroyComponent(Component *component);

    std::vector<Component*> mComponentsForFirstUpdate;
    Camera *mCamera = nullptr;
};

Scene &getScene();

}
