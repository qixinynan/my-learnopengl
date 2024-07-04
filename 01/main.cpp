#include <glad/glad.h>
// GLFW must be included after glad
#include "filesystem.h"
#include "logger.h"
#include "shader.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
  glEnable(GL_DEPTH_TEST);
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
  if (shader.Compile() != 0) {
    LOG_CRITICAL("Failed to compile shaders");
    return -1;
  }

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  glm::vec3 cube_pos_list[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  // ---   RENDER START  ---
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind the VBO & VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // ---   RENDER END    ---

  // Load textures
  uint texture1, texture2;
  LoadTexture(texture1, "resources/textures/container.jpg", GL_RGB);
  LoadTexture(texture2, "resources/textures/awesomeface.png", GL_RGBA);

  shader.Use();
  shader.SetInt("texture1", 0);
  shader.SetInt("texture2", 1);

  if (kLineMode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Main Loop
  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.Use();
    glBindVertexArray(VAO);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection =
        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.SetMatrix4("view", glm::value_ptr(view));
    shader.SetMatrix4("projection", glm::value_ptr(projection));

    for (uint i = 0; i < 10; i++) {
      glm::mat4 local = glm::mat4(1.0f);
      local = glm::translate(local, cube_pos_list[i]);
      float angle = 20.0 * i;
      local =
          glm::rotate(local, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.SetMatrix4("local", glm::value_ptr(local));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

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
