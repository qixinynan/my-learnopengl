#include <glad/glad.h>
// GLFW must be included after glad
#include "app.h"
#include "logger.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <iostream>

const bool kLineMode = true;

void FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int InitWindow(GLFWwindow **window, int width, int height, const char *title) {
  // Init GLFW
  if (!glfwInit()) {
    LOG_CRITICAL("Failed to initialize GLFW");
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create window
  *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (*window == nullptr) {
    LOG_CRITICAL("Failed to create window");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(*window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_CRITICAL("Failed to initialize GLAD");
    return -1;
  }
  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(*window, FrameBufferSizeCallback);
  return 0;
}

void ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void MainLoop(GLFWwindow *window, Shader shader, unsigned int VAO) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.Use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void Terminate() { glfwTerminate(); }

int main() {
  GLFWwindow *window = nullptr;
  InitWindow(&window, 800, 600, "Hello OpenGL");

  Shader shader("shaders/basic.vert", "shaders/basic.frag");
  shader.Compile();

  float vertices[] = {
      0.5f,  0.5f,  0.0f, // 右上角
      0.5f,  -0.5f, 0.0f, // 右下角
      -0.5f, -0.5f, 0.0f, // 左下角
      -0.5f, 0.5f,  0.0f  // 左上角
  };

  unsigned int indices[] = {
      0, 1, 3, // 第一个三角形
      1, 2, 3  // 第二个三角形
  };

  // ---   RENDER START  ---
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Unbind the VBO & VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // ---   RENDER END    ---

  if (kLineMode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  MainLoop(window, shader, VAO);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VAO);

  Terminate();
  return 0;
}
