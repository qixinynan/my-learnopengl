#include "shader.h"
#include "filesystem.h"
#include "logger.h"
#include "types.h"
#include <filesystem>
#include <fstream>

#include <iostream>
#include <sstream>

Shader::Shader(const char *vertex_path, const char *fragment_path) {
  std::ifstream vertex_file, fragment_file;
  vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vertex_file.open(FileSystem::GetPath(vertex_path));
    fragment_file.open(FileSystem::GetPath(fragment_path));
    std::stringstream vertex_stream, fragment_stream;

    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();

    vertex_file.close();
    fragment_file.close();

    vertex_source_ = vertex_stream.str();
    fragment_source_ = fragment_stream.str();
    LOG_TRACE(vertex_source_);
    LOG_TRACE(fragment_source_);
  } catch (std::ifstream::failure e) {

    std::string vertex_abs_path =
        std::filesystem::absolute(vertex_path).string();
    std::string fragment_abs_path =
        std::filesystem::absolute(fragment_path).string();
    LOG_CRITICAL("Failed to open shader file: {}", e.what());

    LOG_CRITICAL("Failed to open vertex shader file: {}", vertex_abs_path);

    LOG_CRITICAL("Failed to open fragment shader file: {}", fragment_abs_path);
  }
}

int ShaderStatus(OpenGLShader shader) {
  int success;
  char info_log[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, info_log);
    LOG_CRITICAL("Shader Error: {}", info_log);
    return -1;
  }
  return 0;
}

int Shader::Compile() {
  OpenGLShader vertex = glCreateShader(GL_VERTEX_SHADER);

  const char *vertex_code = vertex_source_.c_str();
  glShaderSource(vertex, 1, &vertex_code, nullptr);
  glCompileShader(vertex);
  ShaderStatus(vertex);

  OpenGLShader fragment = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragment_code = fragment_source_.c_str();
  glShaderSource(fragment, 1, &fragment_code, nullptr);
  glCompileShader(fragment);
  ShaderStatus(fragment);

  program_ = glCreateProgram();
  glAttachShader(program_, vertex);
  glAttachShader(program_, fragment);
  glLinkProgram(program_);

  int success;
  char info[512];
  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_, 512, nullptr, info);

    LOG_CRITICAL("Failed to link shader program: {}", info);
    return -1;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return 0;
}

void Shader::Use() { glUseProgram(program_); }

void Shader::SetBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(program_, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}
void Shader::SetFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void Shader::SetVec2(const std::string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(program_, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void Shader::SetVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(program_, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void Shader::SetVec4(const std::string &name, float x, float y, float z,
                     float w) {
  glUniform4f(glGetUniformLocation(program_, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}