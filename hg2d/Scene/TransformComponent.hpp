#pragma once
#include "SceneSystem.hpp"
#include "glm/glm.hpp"

namespace hg2d {

class TransformComponent final : public AECSComponent {
public:
    TransformComponent(Engine &engine);

    virtual void onSaveLoad(JSONObject &json, bool isLoad) override;

    void translate(const glm::vec2 &offset);
    void scale(const glm::vec2 &scale);
    void rotate(float angle);

    void setPosition(const glm::vec2 &pos);
    void setSize(const glm::vec2 &size);
    void setAngle(float angle);

    const glm::vec2 &getPosition() const;
    const glm::vec2 &getSize() const;
    float getAngle() const;

private:
    glm::vec2 mPos;
    glm::vec2 mSize;
    float mAngle;
};

}
