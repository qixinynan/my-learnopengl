#ifndef APPLICATION_H
#define APPLICATION_H

#include "game.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Application {
public:
  Application(float width, float height, const char *title);
  static Application *Instance();
  int CreateWindow();
  int CloseWindow();
  void DebugMode(bool debug);
  void BindGame(Game *game);
  Game *GetGame();
  GLFWwindow *window = nullptr;
  float width;
  float height;

private:
  static Application *instance_;
  Game *game_ = nullptr;
  const char *title_;

  Postion last_cursor_ = {.x = 400, .y = 300};
  float lastframe_ = 0.0f;
  bool first_mouse_ = true;
  bool debug_mode_ = false;

  static void ScrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset);
  static void MouseCallBack(GLFWwindow *window, double xpos, double ypos);
  static void FrameBufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
};

#endif