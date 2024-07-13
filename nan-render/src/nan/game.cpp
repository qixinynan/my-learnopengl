#include "nan/game.h"
#include "nan/application.h"
#include "nan/camera.h"
#include "nan/gameobject.h"
#include "nan/logger.h"
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include <SDL_video.h>

Application *app = nullptr;

Game::Game() { app = Application::Instance(); }

void Game::SetRenderFunction(std::function<void()> func) {
  render_func_ = func;
}

void Game::AddGameObject(GameObject *obj, GameObjectType type) {
  if (type == kScreenObject) {
    scrren_objects.push_back(obj);
  } else {
    gameobjects.push_back(obj);
  }
}

void Game::ProcessInput() {
  while (SDL_PollEvent(&event_)) {
    switch (event_.type) {
    case SDL_QUIT:
      running_ = false;
      break;
    case SDL_KEYUP:
      if (event_.key.keysym.sym == SDLK_ESCAPE) {
        running_ = false;
      }
      break;
    case SDL_MOUSEMOTION:
      ProcessMouse();
    }
  }
  ProcessKeyboard();
}

void Game::ProcessKeyboard() {
  float camera_speed = 0.01f;
  const Uint8 *key_states = SDL_GetKeyboardState(nullptr);
  if (key_states[SDL_SCANCODE_W])
    Camera::MainCamera()->MoveForward(camera_speed);
  if (key_states[SDL_SCANCODE_S])
    Camera::MainCamera()->MoveForward(-camera_speed);
  if (key_states[SDL_SCANCODE_A])
    Camera::MainCamera()->MoveRight(-camera_speed);
  if (key_states[SDL_SCANCODE_D])
    Camera::MainCamera()->MoveRight(camera_speed);
  if (key_states[SDL_SCANCODE_Q])
    Camera::MainCamera()->MoveUp(-camera_speed);
  if (key_states[SDL_SCANCODE_E])
    Camera::MainCamera()->MoveUp(camera_speed);
}

void Game::ProcessMouse() {
  float x = event_.motion.xrel;
  float y = event_.motion.yrel;
  float offset_x = x;
  float offset_y = -y;

  float sensitivity = 0.1f;
  offset_x *= sensitivity;
  offset_y *= sensitivity;
  Camera::MainCamera()->Rotate(offset_x, offset_y);
}

void Game::Run() {
  running_ = true;
  while (running_) {
    SDL_GL_SwapWindow(app->window);
    ProcessInput();

    // SDL_ShowCursor(SDL_TRUE);
    // SDL_SetWindowGrab(app->window, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_func_();

    float screen_ratio = ((float)app->width) / ((float)app->height);
    projection_perspective =
        glm::perspective(glm::radians(45.0f), screen_ratio, 0.1f, 100.0f);
    projection_ortho = glm::ortho(0.0f, screen_ratio * ortho_size, ortho_size,
                                  0.0f, -1.0f, 100.0f);
    view = Camera::MainCamera()->GetViewMatrix();

    glEnable(GL_DEPTH_TEST);
    for (GameObject *obj : gameobjects) {
      obj->Render();
    }

    glDisable(GL_DEPTH_TEST);
    for (GameObject *obj : scrren_objects) {
      obj->Render();
    }
  }
  app->CloseWindow();
}