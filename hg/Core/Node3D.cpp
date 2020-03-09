#include "Node3D.hpp"
#include "glm/ext.hpp"

namespace hg {

HG_REGISTER_OBJECT(Node3D);

Node3D::Node3D() : mPos(0, 0, 0), mSize(0, 0, 0), mAngles(0, 0, 0) {
}

void Node3D::onSaveLoad(JSONObject &data, bool isLoad) {
    if (isLoad) {
        setPosition(data["position"]);
        setSize(data["size"]);
        setAngles(data["angles"]);
    }
    else {
        data["position"] = getPosition();
        data["size"] = getSize();
        data["angles"] = getAngles();
    }

    BaseClassName::onSaveLoad(data, isLoad);
}

void Node3D::translate(float x, float y, float z) {
    setPosition(mPos.x + x, mPos.y + y, mPos.z + z);
}

void Node3D::translate(const glm::vec3 &offset) {
    translate(offset.x, offset.y, offset.z);
}

void Node3D::scale(float x, float y, float z) {
    setSize(mSize.x + x, mSize.y + y, mSize.z + z);
}

void Node3D::scale(const glm::vec3 &size) {
    scale(size.x, size.y, size.z);
}

void Node3D::rotate(float x, float y, float z) {
    setAngles(mAngles.x + x, mAngles.y + y, mAngles.z + z);
}

void Node3D::rotate(const glm::vec3 &angles) {
    rotate(angles.x, angles.y, angles.z);
}

void Node3D::setPosition(float x, float y, float z) {
    mPos.x = x;
    mPos.y = y;
    mPos.z = z;
}

void Node3D::setPosition(const glm::vec3 &pos) {
    setPosition(pos.x, pos.y, pos.z);
}

void Node3D::setSize(float x, float y, float z) {
    mSize.x = x;
    mSize.y = y;
    mSize.z = z;
}

void Node3D::setSize(const glm::vec3 &size) {
    setSize(size.x, size.y, size.z);
}

void Node3D::setAngles(float x, float y, float z) {
    mAngles.x = x;
    mAngles.y = y;
    mAngles.z = z;
}

void Node3D::setAngles(const glm::vec3 &angles) {
    setAngles(angles.x, angles.y, angles.z);
}

const glm::vec3 &Node3D::getPosition() const {
    return mPos;
}

const glm::vec3 &Node3D::getSize() const {
    return mSize;
}

const glm::vec3 &Node3D::getAngles() const {
    return mAngles;
}

glm::vec3 Node3D::getAbsolutePosition() const {
    auto rotatedPos = [](const glm::vec3 &p, const glm::vec3 &a) {
        glm::mat4 rot = glm::rotate(a.x, glm::vec3(1, 0, 0))*glm::rotate(a.y, glm::vec3(0, 1, 0))*glm::rotate(a.z, glm::vec3(0, 0, 1));
        return glm::vec3(glm::vec4(p, 1)*rot);
    };

    auto pos = glm::vec3(0, 0, 0);
    const Node3D *node = this;
    while (node) {
        const Node3D *parent = node->getParent() ? node->getParent()->as<Node3D>() : nullptr;
        if (parent) {
            pos += rotatedPos(node->getPosition(), parent->getAngles());
        }
        else {
            pos += node->getPosition();
        }
        node = parent;
    }
    return pos;
}

}
