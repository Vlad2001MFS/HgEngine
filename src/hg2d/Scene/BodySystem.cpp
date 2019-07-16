#include "BodySystem.hpp"
#include "TransformSystem.hpp"

namespace hg2d {

HD_FORCEINLINE const b2Vec2 &toBox2D(const glm::vec2 &v) {
    return reinterpret_cast<const b2Vec2&>(v);
}

HD_FORCEINLINE const glm::vec2 &fromBox2D(const b2Vec2 &v) {
    return reinterpret_cast<const glm::vec2&>(v);
}

BodyComponent::BodyComponent() : mBoxShapeSize(0, 0) {
    mBody = nullptr;
    mFixture = nullptr;
}

void BodyComponent::setPosition(const glm::vec2 &pos) {
    mBody->SetTransform(toBox2D(pos), mBody->GetAngle());
}

void BodyComponent::setAngle(float angle) {
    mBody->SetTransform(mBody->GetPosition(), angle);
}

void BodyComponent::setLinearVelocity(const glm::vec2 &vel) {
    mBody->SetLinearVelocity(toBox2D(vel));
}

void BodyComponent::setAngularVelocity(float vel) {
    mBody->SetAngularVelocity(vel);
}

void BodyComponent::setLinearDamping(float damping) {
    mBody->SetLinearDamping(damping);
}

void BodyComponent::setAngularDamping(float damping) {
    mBody->SetAngularDamping(damping);
}

void BodyComponent::setSleepAllowed(bool allowed) {
    mBody->SetSleepingAllowed(allowed);
}

void BodyComponent::setAwake(bool awake) {
    mBody->SetAwake(awake);
}

void BodyComponent::setFixedRotation(bool fixedRotation) {
    mBody->SetFixedRotation(fixedRotation);
}

void BodyComponent::setBullet(bool bullet) {
    mBody->SetBullet(bullet);
}

void BodyComponent::setType(BodyType type) {
    mBody->SetType(static_cast<b2BodyType>(type));
}

void BodyComponent::setActive(bool active) {
    mBody->SetActive(active);
}

void BodyComponent::setGravityScale(float scale) {
    mBody->SetGravityScale(scale);
}

void BodyComponent::setBoxShape(const glm::vec2& size) {
    mBoxShapeSize = size;
    b2PolygonShape shape;
    shape.SetAsBox(size.x, size.y);
    b2FixtureDef fd;
    fd.shape = &shape;
    mFixture = mBody->CreateFixture(&fd);
}

void BodyComponent::setShapeDensity(float density) {
    if (mFixture) {
        mFixture->SetDensity(density);
    }
}

void BodyComponent::setShapeFriction(float friction) {
    if (mFixture) {
        mFixture->SetFriction(friction);
    }
}

void BodyComponent::setShapeRestitution(float restitution) {
    if (mFixture) {
        mFixture->SetRestitution(restitution);
    }
}

void BodyComponent::setShapeSensor(bool sensor) {
    if (mFixture) {
        mFixture->SetSensor(sensor);
    }
}

const glm::vec2& BodyComponent::getBoxShapeSize() const {
    return mBoxShapeSize;
}

const glm::vec2 &BodyComponent::getPosition() const {
    return fromBox2D(mBody->GetPosition());
}

float BodyComponent::getAngle() const {
    return mBody->GetAngle();
}

const glm::vec2 &BodyComponent::getLinearVelocity() const {
    return fromBox2D(mBody->GetLinearVelocity());
}

float BodyComponent::getAngularVelocity() const {
    return mBody->GetAngularVelocity();
}

float BodyComponent::getLinearDamping() const {
    return mBody->GetLinearDamping();
}

float BodyComponent::getAngularDamping() const {
    return mBody->GetAngularDamping();
}

bool BodyComponent::isSleepAllowed() const {
    return mBody->IsSleepingAllowed();
}

bool BodyComponent::isAwake() const {
    return mBody->IsAwake();
}

bool BodyComponent::isFixedRotation() const {
    return mBody->IsFixedRotation();
}

bool BodyComponent::isBullet() const {
    return mBody->IsBullet();
}

BodyType BodyComponent::getType() const {
    return static_cast<BodyType>(mBody->GetType());
}

bool BodyComponent::isActive() const {
    return mBody->IsActive();
}

float BodyComponent::getGravityScale() const {
    return mBody->GetGravityScale();
}

float BodyComponent::getShapeDensity() const {
    if (mFixture) {
        return mFixture->GetDensity();
    }
    else {
        return 0.0f;
    }
}

float BodyComponent::getShapeFriction() const {
    if (mFixture) {
        return mFixture->GetFriction();
    }
    else {
        return 0.0f;
    }
}

float BodyComponent::getShapeRestitution() const {
    if (mFixture) {
        return mFixture->GetRestitution();
    }
    else {
        return 0.0f;
    }
}

bool BodyComponent::isShapeSensor() const {
    if (mFixture) {
        return mFixture->IsSensor();
    }
    else {
        return false;
    }
}

BodySystem::BodySystem(Engine &engine) : AECSSystem(engine), mWorld(b2Vec2(0, 0)) {
}

void BodySystem::onCreateComponent(AECSComponent* component, uint64_t typeHash) {
    if (typeHash == typeid(BodyComponent).hash_code()) {
        BodyComponent *body = static_cast<BodyComponent*>(component);
        b2BodyDef bd;
        body->mBody = mWorld.CreateBody(&bd);
    }
}

void BodySystem::onDestroyComponent(AECSComponent* component, uint64_t typeHash) {
    if (typeHash == typeid(BodyComponent).hash_code()) {
        BodyComponent *body = static_cast<BodyComponent*>(component);
        mWorld.DestroyBody(body->mBody);
    }
}

void BodySystem::onFixedUpdate() {
    const std::vector<TransformComponent*> &transforms = mSceneSystem.getComponents<TransformComponent>();
    const std::vector<BodyComponent*> &bodies = mSceneSystem.getComponents<BodyComponent>();
    for (size_t i = 0; i < transforms.size(); i++) {
        TransformComponent *transform = transforms[i];
        BodyComponent *body = bodies[i];
        if (transform && body) {
            transform->setPosition(body->getPosition());
            transform->setSize(body->getBoxShapeSize());
            transform->setAngle(body->getAngle());
        }
    }
    mWorld.Step(1.0f / 30.0f, 6, 2);
}

void BodySystem::setGravity(const glm::vec2 &gravity) {
    mWorld.SetGravity(toBox2D(gravity));
}

const glm::vec2 &BodySystem::getGravity() const {
    return fromBox2D(mWorld.GetGravity());
}

}
