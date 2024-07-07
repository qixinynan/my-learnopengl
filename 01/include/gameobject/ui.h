#ifndef UI_H
#define UI_H

#include "application.h"
#include "game.h"
#include "gameobject.h"
#include "logger.h"
#include "shader.h"
#include "types.h"
#include <iostream>

class UI : public GameObject {
public:
  float depth = 0.0f;
  UI() : GameObject() {
    shader_ = Shader("resources/shaders/2d.vert", "resources/shaders/2d.frag");
  }
  const std::vector<float> &GetVertices(uint &step) const override {
    step = 4;
    return vertices_;
  }
  void SetTexture(Texture texture) { GameObject::SetTexture(0, texture); }

  void InitVertexAttribArray() override {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid *)0);
  }

  void UpdateUniforms() override {
    glm::mat4 local = glm::mat4(1.0f);
    local = glm::translate(local, postion);
    local = glm::scale(local, size);
    local = glm::rotate(local, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    shader_.Use();
    shader_.SetMat4("local", local);
    shader_.SetMat4("projection",
                    glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f));
    shader_.SetMat4("view", glm::mat4(1.0f));
  }

private:
  std::vector<float> vertices_ = {
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
};
#endif