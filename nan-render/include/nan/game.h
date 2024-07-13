#ifndef GAME_H
#define GAME_H

#include "nan/camera.h"
#include "nan/gameobject.h"
#include "nan/position.h"
#include <SDL2/SDL.h>
#include <functional>
#include <vector>

class Game {
public:
  float deltatime = 0.0f;
  glm::mat4 projection_perspective = glm::mat4(1.0f);
  glm::mat4 projection_ortho = glm::mat4(1.0f);
  const float ortho_size = 5.0f;
  glm::mat4 view = glm::mat4(1.0f);

  Game();
  void SetRenderFunction(std::function<void()> func);
  void AddGameObject(GameObject *obj, GameObjectType type = kSpaceObject);
  void ProcessInput();
  void Run();

private:
  bool running_ = false;

  // TODO: Event manager
  SDL_Event event_;

  std::function<void()> render_func_ = []() {};
  std::vector<GameObject *> gameobjects = {};
  std::vector<GameObject *> scrren_objects = {};

  void ProcessKeyboard();
  void ProcessMouse();
};
#endif