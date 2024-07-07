#include "application.h"
#include "game.h"
#include "logger.h"

Application *Application::instance_ = nullptr;

Application::Application(float width, float height, const char *title)
    : width(width), height(height), title_(title) {
  instance_ = this;
  // game_ = new Game();
}

void Application::BindGame(Game *game) { this->game_ = game; }

Game *Application::GetGame() { return game_; }

Application *Application::Instance() { return instance_; }

void Application::DebugMode(bool debug) { this->debug_mode_ = debug; }

int Application::CreateWindow() {
  if (!glfwInit()) {
    LOG_CRITICAL("Failed to initialize GLFW");
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(width, height, title_, nullptr, nullptr);
  if (window == nullptr) {
    LOG_CRITICAL("Failed to create window");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_CRITICAL("Failed to initialize GLAD");
    return -1;
  }
  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, MouseCallBack);
  glfwSetScrollCallback(window, ScrollCallback);
  if (debug_mode_)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  LOG_INFO("Window has been created");
  return 0;
}

int Application::CloseWindow() {
  glfwTerminate();
  return 0;
}

void Application::FrameBufferSizeCallback(GLFWwindow *_window, int width,
                                          int height) {
  glViewport(0, 0, width, height);
}

void Application::MouseCallBack(GLFWwindow *_window, double xpos, double ypos) {
  Application *self = Application::Instance();
  if (self->first_mouse_) // 这个bool变量初始时是设定为true的
  {

    self->last_cursor_.x = xpos;
    self->last_cursor_.y = ypos;
    self->first_mouse_ = false;
  }

  float xoffset = xpos - self->last_cursor_.x;
  float yoffset = self->last_cursor_.y - ypos;
  self->last_cursor_.x = xpos;
  self->last_cursor_.y = ypos;
  float sensitivity = 0.05f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;
  Camera::MainCamera()->Rotate(xoffset, yoffset);
}

void Application::ScrollCallback(GLFWwindow *_window, double xoffset,
                                 double yoffset) {
  Application *self = Application::Instance();
  if (Camera::MainCamera()->fov >= 1.0f && Camera::MainCamera()->fov <= 45.0f)
    Camera::MainCamera()->fov -= yoffset;
  if (Camera::MainCamera()->fov <= 1.0f)
    Camera::MainCamera()->fov = 1.0f;
  if (Camera::MainCamera()->fov >= 45.0f)
    Camera::MainCamera()->fov = 45.0f;
}