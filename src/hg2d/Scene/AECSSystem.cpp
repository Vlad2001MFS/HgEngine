#include "AECSSystem.hpp"

namespace hg2d {

AECSSystem::AECSSystem(Engine &engine) : mEngine(engine) {
}

AECSSystem::~AECSSystem() {
}

void AECSSystem::onInitialize() {
}

void AECSSystem::onShutdown() {
}

void AECSSystem::onEvent(const std::vector<GameObject*> &objects, const hd::WindowEvent &event) {
}

void AECSSystem::onFixedUpdate(const std::vector<GameObject*> &objects) {
}

void AECSSystem::onUpdate(const std::vector<GameObject*> &objects) {
}

void AECSSystem::onDraw(const std::vector<GameObject*> &objects) {
}

}
