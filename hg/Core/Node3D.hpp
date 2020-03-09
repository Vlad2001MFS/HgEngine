#pragma once
#include "Node.hpp"
#include "glm/glm.hpp"

namespace hg {

class Node3D : public Node {
    HG_OBJECT(Node3D, Node);
public:
    Node3D();

    virtual void onSaveLoad(JSONObject &data, bool isLoad) override;

    void translate(float x, float y, float z);
    void translate(const glm::vec3 &offset);
    void scale(float x, float y, float z);
    void scale(const glm::vec3 &size);
    void rotate(float x, float y, float z);
    void rotate(const glm::vec3 &angles);

    void setPosition(float x, float y, float z);
    void setPosition(const glm::vec3 &pos);
    void setSize(float x, float y, float z);
    void setSize(const glm::vec3 &size);
    void setAngles(float x, float y, float z);
    void setAngles(const glm::vec3 &angles);

    const glm::vec3 &getPosition() const;
    const glm::vec3 &getSize() const;
    const glm::vec3 &getAngles() const;
    glm::vec3 getAbsolutePosition() const;

private:
    glm::vec3 mPos, mSize, mAngles;
};

}
