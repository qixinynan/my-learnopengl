#include <glad/glad.h>
//
#include "filesystem.h"
#include "stb_image.h"
#include "texture.h"
#include <GLFW/glfw3.h>

Texture::Texture(const char *path, GLint mode) {
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nr_channels;
  stbi_set_flip_vertically_on_load(true);
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
  glActiveTexture(texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_);
}