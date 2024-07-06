#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "shader.h"
#include "texture.h"
#include "types.h"
#include <vector>

class GameObject {
public:
  glm::vec3 postion = glm::vec3(0.0f, 0.0f, 0.0f);
  GameObject(Shader &shader) : shader_(shader){};
  virtual const std::vector<float> &GetVertices() const = 0;
  void SetTexture(uint id, Texture texture);
  void Init();
  void Render();

protected:
  uint VAO_;
  Texture textures_[16];
  Shader &shader_;
};

#endif