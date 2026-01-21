#pragma once

#include <glm/glm.hpp>

enum CameraMovement {
  FORWARD, BACKWARD, LEFT, RIGHT
};

void initCamera(glm::vec3 pos, glm::vec3 up);
void processKeyboard(CameraMovement movement, double deltaTime);
void processMouse(float x, float y);
glm::mat4 getViewMatrix();
