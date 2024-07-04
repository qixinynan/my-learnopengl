#include <glad/glad.h>
// GLFW must be included after glad
#include "camera.h"
#include "filesystem.h"
#include "logger.h"
#include "shader.h"
#include "stb_image.h"
#include "texture.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const bool kLineMode = false;

struct Postion {
  float x;
  float y;
};

Camera camera;          // TODO
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

bool first_mouse = true;

Postion last_cursor = {.x = 400, .y = 300};
float fov = 45.0f;

void FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void MouseCallBack(GLFWwindow *window, double xpos, double ypos) {

  if (first_mouse) // 这个bool变量初始时是设定为true的
  {

    last_cursor.x = xpos;
    last_cursor.y = ypos;
    first_mouse = false;
  }

  float xoffset = xpos - last_cursor.x;
  float yoffset = last_cursor.y - ypos;
  last_cursor.x = xpos;
  last_cursor.y = ypos;
  float sensitivity = 0.05f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;
  camera.Rotate(xoffset, yoffset);
}

void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  if (fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
  if (fov <= 1.0f)
    fov = 1.0f;
  if (fov >= 45.0f)
    fov = 45.0f;
  std::cout << fov << std::endl;
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
  glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(*window, MouseCallBack);
  glfwSetScrollCallback(*window, ScrollCallback);
  return 0;
}

void ProcessInput(GLFWwindow *window) {
  float camera_speed = 2.5f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.MoveForward(camera_speed);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.MoveForward(-camera_speed);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.MoveRight(-camera_speed);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.MoveRight(camera_speed);
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    camera.MoveUp(-camera_speed);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    camera.MoveUp(camera_speed);
}

void Terminate() { glfwTerminate(); }

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
  // uint texture1, texture2;
  Texture texture1("resources/textures/container.jpg");
  Texture texture2("resources/textures/awesomeface.png", GL_RGBA);

  shader.Use();
  shader.SetInt("texture1", 0);
  shader.SetInt("texture2", 1);

  if (kLineMode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Main Loop
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    ProcessInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture1.Bind(GL_TEXTURE0);
    texture2.Bind(GL_TEXTURE1);

    shader.Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection =
        glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
    shader.SetMat4("projection", projection);

    glm::mat4 view = camera.GetViewMatrix();
    shader.SetMat4("view", view);

    glBindVertexArray(VAO);
    for (uint i = 0; i < 10; i++) {
      glm::mat4 local = glm::mat4(1.0f);
      local = glm::translate(local, cube_pos_list[i]);
      float angle = 20.0 * i;
      local =
          glm::rotate(local, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.SetMat4("local", local);
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
