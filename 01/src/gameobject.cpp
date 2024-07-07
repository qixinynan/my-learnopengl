#include "gameobject.h"
#include "application.h"
#include "game.h"
#include "logger.h"
#include <iostream>

void GameObject::SetTexture(uint id, Texture texture) {
  if (id > 15) {
    LOG_ERROR("The max textures number is 16");
    return;
  }
  textures_[id] = texture;
}

void GameObject::GenerateVAO() {
  uint VBO, step;
  auto vertices = GetVertices(step);
  LOG_INFO("GameObject Init");
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  LOG_INFO("sizeof {}", sizeof(vertices));
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);
}

void GameObject::InitVertexAttribArray() {
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void GameObject::InitShader() {
  shader_.Compile();
  shader_.Use();
  shader_.SetInt("texture1", 0);
  shader_.SetInt("texture2", 1);
  shader_.SetInt("texture3", 2);
  shader_.SetInt("texture4", 3);
  shader_.SetInt("texture5", 4);
  shader_.SetInt("texture6", 5);
  shader_.SetInt("texture7", 6);
  shader_.SetInt("texture8", 7);
  shader_.SetInt("texture9", 8);
  shader_.SetInt("texture10", 9);
  shader_.SetInt("texture11", 10);
  shader_.SetInt("texture12", 11);
  shader_.SetInt("texture13", 12);
  shader_.SetInt("texture14", 13);
  shader_.SetInt("texture15", 14);
  shader_.SetInt("texture16", 15);
}

void GameObject::UnBind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void GameObject::Init() {
  GenerateVAO();
  InitVertexAttribArray();
  InitShader();
  UnBind();
}

void GameObject::InitRender() {
  shader_.Use();
  glBindVertexArray(VAO_);
}

void GameObject::BindTextures() {
  textures_[0].Bind(GL_TEXTURE0);
  textures_[1].Bind(GL_TEXTURE1);
  textures_[2].Bind(GL_TEXTURE2);
  textures_[3].Bind(GL_TEXTURE3);
  textures_[4].Bind(GL_TEXTURE4);
  textures_[5].Bind(GL_TEXTURE5);
  textures_[6].Bind(GL_TEXTURE6);
  textures_[7].Bind(GL_TEXTURE7);
  textures_[8].Bind(GL_TEXTURE8);
  textures_[9].Bind(GL_TEXTURE9);
  textures_[10].Bind(GL_TEXTURE10);
  textures_[11].Bind(GL_TEXTURE11);
  textures_[12].Bind(GL_TEXTURE12);
  textures_[13].Bind(GL_TEXTURE13);
  textures_[14].Bind(GL_TEXTURE14);
  textures_[15].Bind(GL_TEXTURE15);
}

void GameObject::UpdateUniforms() {
  glm::mat4 local = glm::mat4(1.0f);
  local = glm::translate(local, postion);
  local = glm::scale(local, size);
  local = glm::rotate(local, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

  shader_.SetMat4("local", local);
  shader_.SetMat4("projection", Application::Instance()->GetGame()->projection);
  shader_.SetMat4("view", Application::Instance()->GetGame()->view);
}

void GameObject::RenderObject() {
  glBindVertexArray(VAO_);
  uint step;
  int size = GetVertices(step).size();
  glDrawArrays(GL_TRIANGLES, 0, size / step);
}

void GameObject::Render() {
  BindTextures();
  InitRender();
  UpdateUniforms();
  RenderObject();
}