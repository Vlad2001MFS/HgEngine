#pragma once
#include "JSONObject.hpp"
#include "Object.hpp"
#include "hd/Core/Common.hpp"
#include "hd/Core/Log.hpp"
#include "hd/Core/StringHash.hpp"
#include "SDL2/SDL_events.h"
#include "glm/glm.hpp"
#include "nameof/nameof.hpp"
#include <vector>
#include <unordered_map>

namespace hg {

class Node : public Object {
    HG_OBJECT(Node, Object);
public:
    Node();
    ~Node();

    virtual void onSaveLoad(JSONObject &data, bool isLoad);
    virtual void onEvent(const SDL_Event &event);
    virtual void onFixedUpdate();
    virtual void onUpdate(float dt);
    virtual void onDraw();

    Node *findByName(const std::string &name);

    template<typename T>
    T *createChild(const std::string &name) {
        T *node = new T();
        mAddChild(node, name);
        return node;
    }

    template<typename T = Node>
    T *createChild(const std::string &name, const std::string &typeName) {
        T *node = Factory::get().createObject(typeName)->as<T>();
        mAddChild(node, name);
        return node;
    }

    template<typename T = Node>
    T *createChild(const std::string &name, const hd::StringHash &typeHash) {
        T *node = Factory::get().createObject(typeHash)->as<T>();
        mAddChild(node, name);
        return node;
    }

    void destroyChild(const std::string &name);

    void setActive(bool active);

    Node *getParent() const;
    const std::vector<Node*> &getChildren() const;
    const std::unordered_map<hd::StringHash, Node*> &getChildrenByNames() const;
    const std::string &getName() const;
    bool isActive() const;

private:
    void mAddChild(Node *node, const std::string &name);

    Node *mParent;
    std::vector<Node*> mChildren;
    std::unordered_map<hd::StringHash, Node*> mChildrenByNames;
    std::string mName;
    bool mIsActive;
};

}
