#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
  Camera(glm::vec3 postion = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
      : position_(postion), up_(up), front_(glm::vec3(0.0f, 0.0f, -1.0f)),
        yaw_(-90.0f), pitch_(0.0f) {}
  glm::mat4 GetViewMatrix();

  void MoveForward(float delta);
  void MoveRight(float delta);
  void MoveUp(float delta);
  void Rotate(float xoffset, float yoffset);

private:
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  float yaw_;
  float pitch_;
};

#endif