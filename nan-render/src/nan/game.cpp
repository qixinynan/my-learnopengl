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
#include <SDL_timer.h>
#include <SDL_video.h>

// TODO: Instance ID

Application *app = nullptr;

Game::Game() { app = Application::Instance(); }

void Game::SetRenderFunction(std::function<void()> func) {
  render_func_ = func;
}

void Game::AddGameObject(GameObject *obj, GameObjectType type) {
  if (type == kScreenObject) {
    scrren_objects_.push_back(obj);
  } else {
    gameobjects_.push_back(obj);
  }
}

void Game::ProcessInput() {
  while (SDL_PollEvent(&event_)) {
    GetApplication()->editor->ProcessInput(&event_);
    switch (event_.type) {
    case SDL_QUIT:
      running_ = false;
      break;
    case SDL_WINDOWEVENT:
      if (event_.window.event == SDL_WINDOWEVENT_RESIZED) {
        int width = event_.window.data1;
        int height = event_.window.data2;
        GetApplication()->width = width;
        GetApplication()->height = height;
        glViewport(0, 0, width, height);
      }
      break;
    case SDL_KEYUP:
      if (event_.key.keysym.sym == SDLK_ESCAPE) {
        running_ = false;
      } else if (event_.key.keysym.sym == SDLK_LALT ||
                 event_.key.keysym.sym == SDLK_RALT) {
        editor_mode_ = !editor_mode_;
      }
      break;
    case SDL_MOUSEMOTION:
      if (!editor_mode_)
        ProcessMouse();
    }
  }
  if (!editor_mode_) {
    ProcessKeyboard();
  }
}

void Game::ProcessKeyboard() {
  float camera_speed = 0.01f;
  const Uint8 *key_states = SDL_GetKeyboardState(nullptr);
  if (key_states[SDL_SCANCODE_W])
    MainCamera()->MoveForward(camera_speed);
  if (key_states[SDL_SCANCODE_S])
    MainCamera()->MoveForward(-camera_speed);
  if (key_states[SDL_SCANCODE_A])
    MainCamera()->MoveRight(-camera_speed);
  if (key_states[SDL_SCANCODE_D])
    MainCamera()->MoveRight(camera_speed);
  if (key_states[SDL_SCANCODE_Q])
    MainCamera()->MoveUp(-camera_speed);
  if (key_states[SDL_SCANCODE_E])
    MainCamera()->MoveUp(camera_speed);
}

void Game::ProcessMouse() {
  float x = event_.motion.xrel;
  float y = event_.motion.yrel;
  float offset_x = x;
  float offset_y = -y;

  float sensitivity = 0.1f;
  offset_x *= sensitivity;
  offset_y *= sensitivity;
  MainCamera()->Rotate(offset_x, offset_y);
}

void Game::Run() {
  running_ = true;
  float frame_start;
  const float frame_delay = 1000.0f / target_fps_;

  while (running_) {
    frame_start = SDL_GetTicks();

    if (editor_mode_) {
      SDL_SetRelativeMouseMode(SDL_FALSE);
    } else {
      SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    SDL_GL_SwapWindow(app->window);
    ProcessInput();

    for (Camera *camera : cameras_) {
      camera->Update();
    }

    GetApplication()->editor->NewFrame();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_func_();

    glEnable(GL_DEPTH_TEST);
    for (GameObject *obj : gameobjects_) {
      obj->Render();
    }

    glDisable(GL_DEPTH_TEST);
    for (GameObject *obj : scrren_objects_) {
      obj->Render();
    }

    if (editor_mode_)
      GetApplication()->editor->Draw();

    deltatime = SDL_GetTicks() - frame_start;
    if (frame_delay > deltatime) {
      SDL_Delay(frame_delay - deltatime);
    }
  }
  app->CloseWindow();
}

void Game::AddCamera(Camera *camera) { cameras_.push_back(camera); }

std::vector<Camera *> Game::GetCameras() { return cameras_; }

Camera *Game::MainCamera() { return main_camera_; }

void Game::SetMainCamera(Camera *main_camera) { main_camera_ = main_camera; }
