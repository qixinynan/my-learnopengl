#ifndef SPRITE_H
#define SPRITE_H
#include "nan/texture.h"

class Sprite {
public:
  Sprite(Texture &texture) : texture_(texture) {}
  void SetSliceCount(float x, float y);
  void SetSlicePosition(float x, float y);
  glm::mat4 GetTextureMatrix();
  Texture &Texture() { return texture_; }

private:
  class Texture texture_;
  Vector<float, 2> slice_count_ = Vector<float, 2>(1, 1);
  Vector<float, 2> slice_position_ = Vector<float, 2>(0, 0);
};

#endif
