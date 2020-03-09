#pragma once
#include "../Core/Node.hpp"
#include "glm/glm.hpp"

namespace hg {

class Node2D : public Node {
    HG_OBJECT(Node2D, Node);
public:
    Node2D();

    virtual void onSaveLoad(JSONObject &data, bool isLoad) override;

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

    const glm::vec2 &getPosition() const;
    const glm::vec2 &getSize() const;
    float getAngle() const;
    glm::vec2 getAbsolutePosition() const;

private:
    glm::vec2 mPos, mSize;
    float mAngle;
};

}
