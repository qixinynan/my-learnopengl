#ifndef SPRITE_H
#define SPRITE_H

#include "nan/application.h"
#include "nan/game.h"
#include "nan/gameobject.h"
#include "nan/logger.h"
#include "nan/shader.h"
#include "nan/types.h"
#include <iostream>

class Sprite : public GameObject {
public:
  Sprite() : GameObject() {
    shader_ = Shader("resources/shaders/2d.vert", "resources/shaders/2d.frag");
  }
  const std::vector<float> &GetVertices(uint &step) const override {
    step = 4;
    return vertices_;
  }
  void SetTexture(Texture texture) {
    GameObject::SetTexture(0, texture);
    size = glm::vec3(texture.width, texture.height, 0.0f);
  }

  void InitVertexAttribArray() override {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid *)0);
  }

  bool gui_mode = false;

  void UpdateUniforms() override {
    glm::mat4 local = glm::mat4(1.0f);
    local = glm::translate(local, postion);
    local = glm::scale(local, size);
    local = glm::rotate(local, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 texmat = glm::mat4(1.0f);

    float unit_x = 1.0f / textures_->slice_count.x();
    float unit_y = 1.0f / textures_->slice_count.y();

    texmat = glm::translate(
        texmat, glm::vec3(unit_x * textures_->slice_position.x(),
                          unit_y * textures_->slice_position.y(), 0.0f));

    texmat = glm::scale(texmat, glm::vec3(unit_x, unit_y, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    if (!gui_mode) {
      view = GetGame()->MainCamera()->GetViewMatrix();
    }

    shader_.Use();
    shader_.SetMat4("texMat", texmat);
    shader_.SetMat4("local", local);

    shader_.SetMat4("projection",
                    GetGame()->MainCamera()->GetProjectionMatrix());
    shader_.SetMat4("view", view);
  }

private:
  std::vector<float> vertices_ = {
      0.0f, 1.0f, 0.0f, 1.0f, //
      1.0f, 0.0f, 1.0f, 0.0f, //
      0.0f, 0.0f, 0.0f, 0.0f, //
      0.0f, 1.0f, 0.0f, 1.0f, //
      1.0f, 1.0f, 1.0f, 1.0f, //
      1.0f, 0.0f, 1.0f, 0.0f  //
  };
};
#endif