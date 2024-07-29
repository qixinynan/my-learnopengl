#include "nan/sprite.h"

void Sprite::SetSliceCount(float x, float y) {
  slice_count_ = Vector<float, 2>(x, y);
}

void Sprite::SetSlicePosition(float x, float y) {
  slice_position_ = Vector<float, 2>(x, y);
}

glm::mat4 Sprite::GetTextureMatrix() {
  glm::mat4 texmat = glm::mat4(1.0f);

  float unit_x = 1.0f / slice_count_.x();
  float unit_y = 1.0f / slice_count_.y();

  texmat =
      glm::translate(texmat, glm::vec3(unit_x * slice_position_.x(),
                                       unit_y * slice_position_.y(), 0.0f));

  texmat = glm::scale(texmat, glm::vec3(unit_x, unit_y, 0.0f));
  return texmat;
}