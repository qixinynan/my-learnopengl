#include <glad/glad.h>
// GLFW must be included after glad
#include "filesystem.h"
#include "logger.h"
#include "shader.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>

const bool kLineMode = false;

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

void Terminate() { glfwTerminate(); }

void LoadTexture(uint &texture, const char *path, GLint mode = GL_RGB) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nr_channels;
  stbi_set_flip_vertically_on_load(true);
  uchar *data = stbi_load(FileSystem::GetPath(path).c_str(), &width, &height,
                          &nr_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode,
                 GL_UNSIGNED_BYTE, data);
  } else {
    LOG_CRITICAL("Failed to load texture");
  }
  stbi_image_free(data);
}

int main() {
  GLFWwindow *window = nullptr;
  InitWindow(&window, 800, 600, "Hello OpenGL");

  Shader shader("shaders/basic.vert", "shaders/basic.frag");
  shader.Compile();

  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  uint indices[] = {0, 1, 3, 1, 2, 3};

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

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Unbind the VBO & VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // ---   RENDER END    ---

  // Load textures
  uint texture1, texture2;
  LoadTexture(texture1, "resources/textures/container.jpg", GL_RGB);
  LoadTexture(texture2, "resources/textures/awesomeface.png", GL_RGBA);

  shader.Use();
  glUniform1i(glGetUniformLocation(shader.GetId(), "texture1"), 0);
  shader.SetInt("texture2", 1);

  if (kLineMode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Main Loop
  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.Use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // End Main Loop

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VAO);

  Terminate();
  return 0;
}
