#ifndef APPLICATION_H
#define APPLICATION_H
#include "SDL2/SDL.h"
#include "nan/game.h"
#include <glad/glad.h>

class Application {
public:
  Application(float width, float height, const char *title);
  static Application *Instance();
  int CreateWindow();
  int CloseWindow();
  void DebugMode(bool debug);
  void BindGame(Game *game);
  Game *GetGame();
  SDL_Window *window = nullptr;
  float width;
  float height;

private:
  static Application *instance_;
  Game *game_ = nullptr;
  const char *title_;
  SDL_GLContext context_;
  float lastframe_ = 0.0f;
  bool debug_mode_ = false;
};

#endif