#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "nan/shader.h"
#include "nan/texture.h"
#include "nan/types.h"
#include <vector>

class GameObject {
public:
  glm::vec3 postion = glm::vec3(0.0f);
  glm::vec3 size = glm::vec3(1.0f);
  GLfloat rotate = 0.0f;
  std::string name = "Unnamed Object";

  GameObject(){};
  virtual const std::vector<float> &GetVertices(uint &step) const = 0;
  void SetTexture(uint id, Texture texture);
  void Init();
  void Render();

protected:
  uint VAO_;
  Texture textures_[16];
  Shader shader_ =
      Shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");

  // Init
  virtual void GenerateVAO();
  virtual void InitVertexAttribArray();
  virtual void InitShader();
  virtual void UnBind();

  // Render
  virtual void InitRender();
  virtual void BindTextures();
  virtual void UpdateUniforms();
  virtual void RenderObject();
};

enum GameObjectType { kSpaceObject, kScreenObject };

#endif