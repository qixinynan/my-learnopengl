#include <glad/glad.h>
//
#include "filesystem.h"
#include "logger.h"
#include "stb_image.h"
#include "texture.h"
#include <GLFW/glfw3.h>

void Texture::LoadTexture(const char *path, GLint mode) {
  enable = true;
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int nr_channels;
  // stbi_set_flip_vertically_on_load(true);
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

void Texture::Bind(GLenum texture_id) {
  if (enable) {
    glActiveTexture(texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_);
  }
}

void Texture::SetSliceCount(float x, float y) {
  slice_count = Vector<float, 2>(x, y);
}

void Texture::SetSlicePosition(float x, float y) {
  slice_position = Vector<float, 2>(x, y);
}