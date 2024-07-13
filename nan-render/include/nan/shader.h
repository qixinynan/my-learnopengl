#ifndef SHADER_H
#define SHADER_H

#include "nan/types.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
  Shader(const char *vertex_path, const char *fragment_path);

  int Compile();
  void Use();
  OpenGLShaderProgram GetId() { return program_; };

  void SetBool(const std::string &name, bool value) const;
  void SetInt(const std::string &name, int value) const;
  void SetFloat(const std::string &name, float value) const;

  void SetVec2(const std::string &name, const glm::vec2 &value) const;
  void SetVec2(const std::string &name, float x, float y) const;
  void SetVec3(const std::string &name, const glm::vec3 &value) const;
  void SetVec3(const std::string &name, float x, float y, float z) const;
  void SetVec4(const std::string &name, const glm::vec4 &value) const;
  void SetVec4(const std::string &name, float x, float y, float z, float w);
  void SetMat2(const std::string &name, const glm::mat2 &mat) const;
  void SetMat3(const std::string &name, const glm::mat3 &mat) const;
  void SetMat4(const std::string &name, const glm::mat4 &mat) const;

private:
  std::string vertex_source_ = "";
  std::string fragment_source_ = "";
  OpenGLShaderProgram program_ = 0;
};

#endif