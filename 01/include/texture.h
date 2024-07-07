#ifndef TEXTURE_H
#define TEXTURE_H

#include "types.h"
#include <glad/glad.h>

class Texture {
public:
  Texture(){};
  Texture(const char *texture_path, GLint mode = GL_RGB) {
    LoadTexture(texture_path, mode);
  };
  void LoadTexture(const char *texture_path, GLint mode = GL_RGB);
  void Bind(GLenum texture_id);
  uint GetTextureId() { return texture_; }

private:
  bool enable = false;
  uint texture_ = -1;
};

#endif