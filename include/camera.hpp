#pragma once

#include <glm/glm.hpp>

struct CameraMovement {
  bool forward = false;
  bool backward = false;
  bool left = false;
  bool right = false;
  bool up = false;
  bool down = false;
  bool fastSpeed = false;
};

void initCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 u);
void updateCamera(double deltaTime, const float mouseX, const float mouseY, CameraMovement movement);
glm::mat4 getViewMatrix();
