#include "GUISystem.hpp"
#include "../Core/Engine.hpp"
#include "hd/Core/Log.hpp"
#include "../../imgui/imgui_impl_sdl.h"
#include "../../imgui/imgui_impl_opengl3.h"

namespace hg {

void GUISystem::initialize() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(getEngine().getWindow(), getEngine().getGLContext());
    ImGui_ImplOpenGL3_Init();
}

void GUISystem::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUISystem::onEvent(const SDL_Event &event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void GUISystem::onUpdate(float dt) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(getEngine().getWindow());
    ImGui::NewFrame();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}
