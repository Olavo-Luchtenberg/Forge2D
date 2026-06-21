#include "editor/Editor.h"
#include <imgui.h>
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vendor/imgui/imgui_impl_opengl3.h>

Entity* Editor::s_selected = nullptr;

bool Editor::init(SDL_Window* window, SDL_GLContext context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 330");

    return true;
}

void Editor::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Editor::processEvent(SDL_Event* event) {
    ImGui_ImplSDL2_ProcessEvent(event);
}

void Editor::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Editor::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::render(Scene* scene) {
    drawEntityPanel(scene);
    if (s_selected) drawInspector(s_selected);
}

void Editor::drawEntityPanel(Scene* scene) {
    ImGui::Begin("Entidades");
    for (auto& entity : scene->getEntities()) {
        bool selected = (s_selected == entity.get());
        if (ImGui::Selectable(entity->name.c_str(), selected))
            s_selected = entity.get();
    }
    ImGui::End();
}

void Editor::drawInspector(Entity* entity) {
    ImGui::Begin("Inspetor");

    ImGui::Text("Nome: %s", entity->name.c_str());
    ImGui::Separator();

    ImGui::Text("Transform");
    ImGui::DragFloat2("Posicao", &entity->transform.position.x, 1.0f);
    ImGui::DragFloat2("Tamanho", &entity->transform.size.x, 1.0f);
    ImGui::DragFloat("Rotacao", &entity->transform.rotation, 0.5f);

    ImGui::Separator();
    ImGui::Text("Sprite");
    ImGui::ColorEdit4("Cor", &entity->sprite.color.x);

    ImGui::Separator();
    ImGui::Checkbox("Ativo", &entity->active);

    ImGui::End();
}