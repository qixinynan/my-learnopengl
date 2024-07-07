#include <glad/glad.h>
// GLFW must be included after glad
#include "application.h"
#include "camera.h"
#include "filesystem.h"
#include "gameobject.h"
#include "gameobject/cube.h"
#include "gameobject/ui.h"
#include "logger.h"
#include "position.h"
#include "shader.h"
#include "stb_image.h"
#include "texture.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main() {
  Application app(800, 600, "Hello World");
  // app.DebugMode(true);
  app.CreateWindow();
  Game game;
  app.BindGame(&game);

  Texture texture1("resources/textures/container.jpg");
  Texture texture2("resources/textures/awesomeface.png", GL_RGBA);
  Texture texture3("resources/textures/awesomeface.png", GL_RGBA);

  Cube cube;
  cube.name = "Cube 0";
  cube.SetTexture(0, texture1);
  cube.SetTexture(1, texture2);
  cube.Init();
  game.AddGameObject(&cube);

  Cube cube1;
  cube1.name = "Cube 1";
  cube1.SetTexture(0, texture1);
  cube1.SetTexture(1, texture2);
  cube1.postion = glm::vec3(1.0f, 0.0f, 0.0f);
  cube1.Init();
  game.AddGameObject(&cube1);

  UI ui;
  ui.name = "UI 0";
  ui.depth = 0.5f;
  ui.SetTexture(texture1);
  ui.postion = glm::vec3(0.0f, 0.0f, 0.0f);
  ui.size = glm::vec3(100.0f, 100.0f, 0.0f);
  ui.Init();
  game.AddGameObject(&ui, kScreenObject);

  UI ui2;
  ui2.name = "UI 1";
  ui2.depth = 0.7f;
  ui2.postion = glm::vec3(0.0f, 0.0f, 0.0f);
  ui2.SetTexture(texture3);
  ui2.size = glm::vec3(100.0f, 100.0f, 0.0f);
  ui2.Init();
  game.AddGameObject(&ui2, kScreenObject);

  Camera camera(glm::vec3(0, 0, 0));
  Camera::SetMainCamera(&camera);

  game.Run();
  app.CloseWindow();
  return 0;
}