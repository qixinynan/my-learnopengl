#include "nan/editor/editor.h"
#include "nan/editor/maineditor.h"

Editor::~Editor() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void Editor::Init() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();

  // 设置ImGui的SDL和OpenGL3实现
  ImGui_ImplSDL2_InitForOpenGL(window_, context_);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void Editor::NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow();
  MainEditor::ShowWindow();
  ImGui::Render();
}

void Editor::Draw() { ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); }

void Editor::ProcessInput(SDL_Event *event) {
  ImGui_ImplSDL2_ProcessEvent(event);
}