#include "BodySystem.hpp"
#include "TransformComponent.hpp"
#include "hd/Core/Common.hpp"

namespace hg2d {

HD_FORCEINLINE b2Vec2 toBox2D(const glm::vec2 &v) {
    return b2Vec2(v.x, v.y);
}

HD_FORCEINLINE glm::vec2 fromBox2D(const b2Vec2 &v) {
    return glm::vec2(v.x, v.y);
}

BodyComponent::BodyComponent() : mBoxShapeSize(0, 0) {
    mBody = nullptr;
    mFixture = nullptr;
}

void BodyComponent::onSaveLoad(JSONObject& json, bool isLoad) {
    JSONObject &jsonPosition = json["position"];
    JSONObject &jsonAngle = json["angle"];
    JSONObject &jsonLinearVelocity = json["linearVelocity"];
    JSONObject &jsonAngularVelocity = json["angularVelocity"];
    JSONObject &jsonLinearDamping = json["linearDamping"];
    JSONObject &jsonAngularDamping = json["angularDamping"];
    JSONObject &jsonSleepAllowed = json["isSleepingAllowed"];
    JSONObject &jsonAwake = json["isAwake"];
    JSONObject &jsonFixedRotation = json["isFixedRotation"];
    JSONObject &jsonBullet = json["isBullet"];
    JSONObject &jsonType = json["type"];
    JSONObject &jsonActive = json["isActive"];
    JSONObject &jsonGravityScale = json["gravityScale"];

    JSONObject &jsonBoxShape = json["boxShapeSize"];
    JSONObject &jsonShapeDensity = json["density"];
    JSONObject &jsonShapeFriction = json["friction"];
    JSONObject &jsonShapeRestitution = json["restitution"];
    JSONObject &jsonShapeSensor = json["isSensor"];
    if (isLoad) {
        setPosition(jsonPosition.get<glm::vec2>());
        setAngle(jsonAngle.get<float>());
        setLinearVelocity(jsonLinearVelocity.get<glm::vec2>());
        setAngularVelocity(jsonAngularVelocity.get<float>());
        setLinearDamping(jsonLinearDamping.get<float>());
        setAngularDamping(jsonAngularDamping.get<float>());
        setSleepAllowed(jsonSleepAllowed.get<bool>());
        setAwake(jsonAwake.get<bool>());
        setFixedRotation(jsonFixedRotation.get<bool>());
        setBullet(jsonBullet.get<bool>());
        setType(jsonType.get<BodyType>());
        setActive(jsonActive.get<bool>());
        setGravityScale(jsonGravityScale.get<float>());

        setBoxShapeSize(jsonBoxShape.get<glm::vec2>());
        setShapeDensity(jsonShapeDensity.get<float>());
        setShapeFriction(jsonShapeFriction.get<float>());
        setShapeRestitution(jsonShapeRestitution.get<float>());
        setShapeSensor(jsonShapeSensor.get<bool>());
    }
    else {
        jsonPosition = getPosition();
        jsonAngle = getAngle();
        jsonLinearVelocity = getLinearVelocity();
        jsonAngularVelocity = getAngularVelocity();
        jsonLinearDamping = getLinearDamping();
        jsonAngularDamping = getAngularDamping();
        jsonSleepAllowed = isSleepAllowed();
        jsonAwake = isAwake();
        jsonFixedRotation = isFixedRotation();
        jsonBullet = isBullet();
        jsonType = getType();
        jsonActive = isActive();
        jsonGravityScale = getGravityScale();

        jsonBoxShape = getBoxShapeSize();
        jsonShapeDensity = getShapeDensity();
        jsonShapeFriction = getShapeFriction();
        jsonShapeRestitution = getShapeRestitution();
        jsonShapeSensor = isShapeSensor();
    }
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

void BodyComponent::setBoxShapeSize(const glm::vec2& size) {
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

glm::vec2 BodyComponent::getPosition() const {
    return fromBox2D(mBody->GetPosition());
}

float BodyComponent::getAngle() const {
    return mBody->GetAngle();
}

glm::vec2 BodyComponent::getLinearVelocity() const {
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

CollisionInfo BodyComponent::getCollisionInfo() const {
    CollisionInfo info;
    info.hasCollision = mFixture->GetUserData() ? true : false;
    if (info.hasCollision) {
        b2Fixture *fixture = static_cast<b2Fixture*>(mFixture->GetUserData());
        info.entity.value = reinterpret_cast<uint64_t>(fixture->GetBody()->GetUserData());
    }
    return info;
}

void ContactListener::BeginContact(b2Contact *contact) {
    b2Fixture *a = contact->GetFixtureA();
    b2Fixture *b = contact->GetFixtureB();
    a->SetUserData(b);
    b->SetUserData(a);
}

void ContactListener::EndContact(b2Contact *contact) {
    b2Fixture *a = contact->GetFixtureA();
    b2Fixture *b = contact->GetFixtureB();
    a->SetUserData(nullptr);
    b->SetUserData(nullptr);
}

BodySystem::BodySystem() : mWorld(nullptr) {
}

void BodySystem::onInitialize() {
    getSceneSystem().registerComponentType<BodyComponent>();

    mWorld = new b2World(b2Vec2(0, 0));
    mWorld->SetContactListener(&mContactListener);
}

void BodySystem::onShutdown() {
    HD_DELETE(mWorld);
}

void BodySystem::onCreateComponent(AECSComponent* component, uint64_t typeHash, const HEntity &entity) {
    if (typeHash == typeid(BodyComponent).hash_code()) {
        BodyComponent *body = static_cast<BodyComponent*>(component);
        b2BodyDef bd;
        body->mBody = mWorld->CreateBody(&bd);
        body->mBody->SetUserData(reinterpret_cast<void*>(entity.value));
    }
}

void BodySystem::onDestroyComponent(AECSComponent* component, uint64_t typeHash, const HEntity &entity) {
    if (typeHash == typeid(BodyComponent).hash_code()) {
        BodyComponent *body = static_cast<BodyComponent*>(component);
        body->mBody->DestroyFixture(body->mFixture);
        mWorld->DestroyBody(body->mBody);
    }
}

void BodySystem::onSaveLoad(JSONObject& json, bool isLoad) {
    JSONObject &jsonGravity = json["gravity"];
    if (isLoad) {
        setGravity(jsonGravity.get<glm::vec2>());
    }
    else {
        jsonGravity = getGravity();
    }
}

void BodySystem::onClear() {
    HD_DELETE(mWorld);
    mWorld = new b2World(b2Vec2(0, 0));
    mWorld->SetContactListener(&mContactListener);
}

void BodySystem::onFixedUpdate() {
    const std::vector<TransformComponent*> &transforms = getSceneSystem().getComponents<TransformComponent>();
    const std::vector<BodyComponent*> &bodies = getSceneSystem().getComponents<BodyComponent>();
    for (size_t i = 0; i < transforms.size(); i++) {
        TransformComponent *transform = transforms[i];
        BodyComponent *body = bodies[i];
        if (transform && body) {
            transform->setPosition(body->getPosition());
            transform->setSize(body->getBoxShapeSize());
            transform->setAngle(body->getAngle());
        }
    }
    mWorld->Step(1.0f / 30.0f, 6, 2);
}

void BodySystem::setGravity(const glm::vec2 &gravity) {
    mWorld->SetGravity(toBox2D(gravity));
}

glm::vec2 BodySystem::getGravity() const {
    return fromBox2D(mWorld->GetGravity());
}

}
