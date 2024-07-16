#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Camera {
public:
  enum ProjectionType { kPerspective, kOrtho };

  Camera(ProjectionType type = kPerspective,
         glm::vec3 postion = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
      : type_(type), position_(postion), up_(up),
        front_(glm::vec3(0.0f, 0.0f, -1.0f)), yaw_(-90.0f), pitch_(0.0f){};

  const float fov = 45.0f;
  const float ortho_size = 5.0f;
  std::string name = "Unnamed Camera";

  void Update();
  glm::mat4 GetViewMatrix() { return view_; }
  glm::mat4 GetProjectionMatrix() { return projection_; }

  void MoveForward(float delta);
  void MoveRight(float delta);
  void MoveUp(float delta);
  void Rotate(float xoffset, float yoffset);

private:
  ProjectionType type_;
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  float yaw_;
  float pitch_;

  glm::mat4 projection_ = glm::mat4(1.0f);
  glm::mat4 view_ = glm::mat4(1.0f);
};
#endif