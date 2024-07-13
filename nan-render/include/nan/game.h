#ifndef GAME_H
#define GAME_H

#include "nan/camera.h"
#include "nan/gameobject.h"
#include "nan/position.h"
#include <SDL2/SDL.h>
#include <functional>
#include <vector>

#define GAME Application::Instance()->GetGame()

class Game {
public:
  float deltatime = 0.0f;

  Game();
  void SetRenderFunction(std::function<void()> func);
  void AddGameObject(GameObject *obj, GameObjectType type = kSpaceObject);

  void AddCamera(Camera *camera);
  Camera *MainCamera();
  void SetMainCamera(Camera *main_camera);

  void ProcessInput();
  void Run();

private:
  // TODO: Event manager
  bool running_ = false;
  SDL_Event event_;
  Camera *main_camera_;

  std::function<void()> render_func_ = []() {};
  std::vector<GameObject *> gameobjects_ = {};
  std::vector<GameObject *> scrren_objects_ = {};
  std::vector<Camera *> cameras_ = {};

  void ProcessKeyboard();
  void ProcessMouse();
};
#endif