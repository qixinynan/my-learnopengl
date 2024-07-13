#include "nan/application.h"
#include "nan/logger.h"

Application *Application::instance_ = nullptr;

Application::Application(float width, float height, const char *title)
    : width(width), height(height), title_(title) {
  instance_ = this;
}

Application *Application::Instance() { return instance_; }

int Application::CreateWindow() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    LOG_CRITICAL("Failed to init SDL2: {}", SDL_GetError());
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  window =
      SDL_CreateWindow(title_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    LOG_CRITICAL("Failed to create window: {}", SDL_GetError());
    CloseWindow();
    return -1;
  }
  context_ = SDL_GL_CreateContext(window);
  if (context_ == nullptr) {
    LOG_CRITICAL("Failed to create OpenGL context: {}", SDL_GetError());
    CloseWindow();
    return -1;
  }
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    LOG_CRITICAL("Failed to initialize GLAD");
    CloseWindow();
    return -1;
  }
  glViewport(0, 0, width, height);
  // glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glfwSetCursorPosCallback(window, MouseCallBack);
  // glfwSetScrollCallback(window, ScrollCallback);
  if (debug_mode_)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  LOG_INFO("Window has been created");
  return 0;
}

int Application::CloseWindow() {
  SDL_GL_DeleteContext(context_);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

void Application::BindGame(Game *game) { this->game_ = game; }

Game *Application::GetGame() { return game_; }

void Application::DebugMode(bool debug) { this->debug_mode_ = debug; }