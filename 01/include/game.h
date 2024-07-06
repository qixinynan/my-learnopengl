#ifndef GAME_H
#define GAME_H

#include "camera.h"
#include "gameobject.h"
#include "glad/glad.h"
#include "position.h"
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>

class Game {
public:
  float deltatime = 0.0f;
  glm::mat4 projection = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);

  Game();
  void SetRenderFunction(std::function<void()> func);
  void AddGameObject(GameObject *obj);
  void ProcessInput(GLFWwindow *window);
  void Run();

private:
  bool debug_ = false;
  std::function<void()> render_func_ = []() {};
  std::vector<GameObject *> gameobjects = {};
};
#endif