#include "AGameState.hpp"

namespace hg2d {

AGameState::AGameState(Engine &engine) : AEngineObject(engine) {
}

void AGameState::onInitialize() {
}

void AGameState::onShutdown() {
}

void AGameState::onEvent(const hd::WindowEvent &event) {
}

void AGameState::onChangeCurrentState(AGameState *lastState) {
}

void AGameState::onFixedUpdate() {
}

void AGameState::onUpdate() {
}

void AGameState::onDraw() {
}

}
