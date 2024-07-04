#ifndef SHADER_H
#define SHADER_H

#include "types.h"
#include <glad/glad.h>
#include <string>

class Shader {
public:
  Shader(const char *vertex_path, const char *fragment_path);

  void Compile();
  void Use();
  OpenGLShaderProgram GetId() { return program_; };

  void SetBool(const std::string &name, bool value) const;
  void SetInt(const std::string &name, int value) const;
  void SetFloat(const std::string &name, float value) const;

private:
  std::string vertex_source_ = "";
  std::string fragment_source_ = "";
  OpenGLShaderProgram program_ = 0;
};

#endif