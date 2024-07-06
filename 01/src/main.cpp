#include <glad/glad.h>
// GLFW must be included after glad
#include "application.h"
#include "camera.h"
#include "filesystem.h"
#include "gameobject.h"
#include "gameobject/cube.h"
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
  app.CreateWindow();

  Game game;
  app.BindGame(&game);

  Shader shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");
  if (shader.Compile() != 0) {
    LOG_CRITICAL("Failed to compile shaders");
    return -1;
  }

  Texture texture1("resources/textures/container.jpg");
  Texture texture2("resources/textures/awesomeface.png", GL_RGBA);

  Cube cube(shader);
  cube.SetTexture(0, texture1);
  cube.SetTexture(1, texture2);
  cube.Init();

  Camera camera(glm::vec3(0, 0, 0));
  Camera::SetMainCamera(&camera);

  // game.SetRenderFunction([]() { LOG_INFO("LALALA"); });
  game.AddGameObject(&cube);
  game.Run();
  app.CloseWindow();
  return 0;
}