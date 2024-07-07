#include "game.h"
#include "application.h"
#include "camera.h"
#include "gameobject.h"

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

void Game::ProcessInput(GLFWwindow *window) {
  float camera_speed = 0.1f;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    Camera::MainCamera()->MoveForward(camera_speed);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    Camera::MainCamera()->MoveForward(-camera_speed);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    Camera::MainCamera()->MoveRight(-camera_speed);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    Camera::MainCamera()->MoveRight(camera_speed);
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    Camera::MainCamera()->MoveUp(-camera_speed);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    Camera::MainCamera()->MoveUp(camera_speed);
}

void Game::Run() {

  while (!glfwWindowShouldClose(app->window)) {
    glfwSwapBuffers(app->window);
    glfwPollEvents();
    ProcessInput(app->window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_func_();
    projection = glm::perspective(glm::radians(45.0f), app->width / app->height,
                                  0.1f, 100.0f);
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
}