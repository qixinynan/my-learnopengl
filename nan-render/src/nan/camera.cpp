#include "nan/camera.h"
#include "nan/application.h"

void Camera::Update() {
  float screen_ratio = ((float)Application::Instance()->width) /
                       ((float)Application::Instance()->height);
  view_ = glm::lookAt(position_, position_ + front_, up_);

  if (type_ == kOrtho) {
    projection_ = glm::ortho(0.0f, screen_ratio * ortho_size, ortho_size, 0.0f,
                             -1.0f, 100.0f);
  } else {
    projection_ =
        glm::perspective(glm::radians(45.0f), screen_ratio, 0.1f, 100.0f);
  }
}

void Camera::MoveForward(float delta) { position_ += delta * front_; }

void Camera::MoveRight(float delta) {
  position_ += delta * (glm::normalize(glm::cross(front_, up_)));
}

void Camera::MoveUp(float delta) { position_ += delta * up_; }

void Camera::Rotate(float xoffset, float yoffset) {
  if (pitch_ > 89.0f)
    pitch_ = 89.0f;
  if (pitch_ < -89.0f)
    pitch_ = -89.0f;
  yaw_ += xoffset;
  pitch_ += yoffset;

  glm::vec3 front;
  front.x = cos(glm::radians(pitch_)) * cos(glm::radians(yaw_));
  front.y = sin(glm::radians(pitch_));
  front.z = cos(glm::radians(pitch_)) * sin(glm::radians(yaw_));
  front_ = glm::normalize(front);
}