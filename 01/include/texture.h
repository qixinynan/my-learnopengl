#ifndef TEXTURE_H
#define TEXTURE_H

#include "types.h"
#include <glad/glad.h>

class Texture {
public:
  Texture(const char *texture_path, GLint mode = GL_RGB);
  void Bind(GLenum texture_id);

private:
  uint texture_ = -1;
};

#endif