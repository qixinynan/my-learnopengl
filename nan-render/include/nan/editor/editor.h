#ifndef EDITOR_H
#define EDITOR_H

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include <SDL2/SDL.h>
#include <glad/glad.h>

class Editor {
public:
  Editor(SDL_Window *window, SDL_GLContext context)
      : window_(window), context_(context) {}
  ~Editor();
  void Init();
  void NewFrame();
  void Draw();
  void ProcessInput(SDL_Event *event);

private:
  SDL_Window *window_;
  SDL_GLContext context_;
};

#endif