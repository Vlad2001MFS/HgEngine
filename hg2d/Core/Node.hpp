#pragma once
#include "Factory.hpp"
#include "hd/Core/Common.hpp"
#include "hd/Core/Log.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/Window/WindowEvent.hpp"
#include "glm/glm.hpp"
#include "nameof/nameof.hpp"
#include <vector>
#include <unordered_map>

namespace hg2d {

class Node : public hd::Noncopyable {
public:
    Node();
    virtual ~Node();

    virtual void onEvent(const hd::WindowEvent &event);
    virtual void onFixedUpdate();
    virtual void onUpdate(float dt);
    virtual void onDraw();

    Node *findByName(const std::string &name);

    template<typename T>
    T *as() {
        T *ptr = dynamic_cast<T*>(this);
        if (!ptr) {
            LOG_F(WARNING, "Failed to cast node to '{}'", NAMEOF_TYPE(T));
        }
        return ptr;
    }

    template<typename T>
    T *createChild(const std::string &name) {
        T *node = new T();
        mAddChild(node, name);
        return node;
    }

    template<typename T = Node>
    T *createChild(const std::string &name, const std::string &typeName) {
        T *node = static_cast<T*>(Factory::get().createObject(typeName));
        mAddChild(node, name);
        return node;
    }

    void destroyChild(const std::string &name);

    void translate(float x, float y);
    void translate(const glm::vec2 &offset);
    void scale(float x, float y);
    void scale(const glm::vec2 &size);
    void rotate(float angle);

    void setPosition(float x, float y);
    void setPosition(const glm::vec2 &pos);
    void setSize(float x, float y);
    void setSize(const glm::vec2 &size);
    void setAngle(float angle);
    void setActive(bool active);

    Node *getParent() const;
    const std::vector<Node*> &getChildren() const;
    const std::unordered_map<uint64_t, Node*> &getChildrenByNames() const;
    const glm::vec2 &getPosition() const;
    const glm::vec2 &getSize() const;
    float getAngle() const;
    bool isActive() const;
    glm::vec2 getAbsolutePosition() const;

private:
    void mAddChild(Node *node, const std::string &name);

    Node *mParent;
    std::vector<Node*> mChildren;
    std::unordered_map<uint64_t, Node*> mChildrenByNames;
    glm::vec2 mPos, mSize;
    float mAngle;
    bool mIsActive;
};

HG2D_REGISTER_OBJECT("Node", Node);

}