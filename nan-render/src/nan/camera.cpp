#include "nan/camera.h"

Camera *Camera::main_camera_ = nullptr;

Camera *Camera::MainCamera() { return main_camera_; }

void Camera::SetMainCamera(Camera *main_camera) { main_camera_ = main_camera; }

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(position_, position_ + front_, up_);
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