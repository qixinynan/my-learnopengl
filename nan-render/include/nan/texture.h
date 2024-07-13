#ifndef TEXTURE_H
#define TEXTURE_H

#include "nan/types.h"
#include "nan/vector.h"
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
  void SetSliceCount(float x, float y);
  void SetSlicePosition(float x, float y);

  int width;
  int height;

  // The slice count of objects in a single image
  Vector<float, 2> slice_count = Vector<float, 2>(1, 1);
  Vector<float, 2> slice_position = Vector<float, 2>(0, 0);

private:
  bool enable = false;
  uint texture_ = -1;
};

#endif