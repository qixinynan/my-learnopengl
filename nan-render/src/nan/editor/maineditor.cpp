#include "nan/editor/maineditor.h"
#include "imgui.h"
#include "nan/application.h"
#include "nan/logger.h"

void MainEditor::ShowWindow() {
  static float f = 0.0f;
  static int counter = 0;

  ImGui::Begin("Main Editor");
  if (ImGui::CollapsingHeader("Camera")) {
    static int selected_camera = -1;
    if (ImGui::TreeNode("Playing Camera")) {
      std::vector<Camera *> cameras = GetGame()->GetCameras();
      for (int i = 0; i < cameras.size(); i++) {
        auto item = cameras[i];
        // LOG_INFO("Camera!: {}", item->name);
        if (ImGui::Selectable(item->name.c_str(), selected_camera == i)) {
          selected_camera = i;
          GetGame()->SetMainCamera(item);
        }
      }
      ImGui::TreePop();
    }
  }
  if (ImGui::CollapsingHeader("Others")) {
    ImGui::Text("This is some useful text.");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    if (ImGui::Button("Button"))
      counter++;

    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
  }
  ImGui::End();
}