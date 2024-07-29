#ifndef TEXTURE_H
#define TEXTURE_H

#include "nan/types.h"
#include "nan/vector.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture {
public:
  Texture(){};
  Texture(const char *texture_path, GLint mode = GL_RGB) {
    LoadTexture(texture_path, mode);
  };
  void LoadTexture(const char *texture_path, GLint mode = GL_RGB);
  void Bind(GLenum texture_id);
  uint GetTextureId() { return texture_; }

  int width;
  int height;

  // The slice count of objects in a single image

private:
  bool enable = false;
  uint texture_ = -1;
};
#endif