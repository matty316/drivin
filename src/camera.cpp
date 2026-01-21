#include <camera.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <constants.hpp>

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;
glm::vec3 cameraRight;
glm::vec3 worldUp;
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
constexpr float cameraSpeed = 2.5f;
constexpr float cameraSensitivity = 0.0001f;
bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

void updateCameraVecs() {
  glm::vec3 front;
  front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
  front.y = sin(glm::radians(cameraPitch));
  front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
  cameraFront = glm::normalize(front);
  cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
  cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void initCamera(glm::vec3 pos, glm::vec3 up) {
  cameraPos = pos;
  worldUp = up;
  updateCameraVecs();
}

void processKeyboard(CameraMovement movement, double deltaTime) {
  float velocity = cameraSpeed * deltaTime;
  if (movement == FORWARD)
    cameraPos += cameraFront * velocity;
  if (movement == BACKWARD)
    cameraPos -= cameraFront * velocity;
  if (movement == LEFT)
    cameraPos -= cameraRight * velocity;
  if (movement == RIGHT)
    cameraPos += cameraRight * velocity;
}

void processMouse(float x, float y) {
  if (firstMouse) {
    lastX = x;
    lastY = y;
    firstMouse = false;
  }

  float xOffset = x - lastX;
  float yOffset = lastY - y;

  xOffset *= cameraSensitivity;
  yOffset *= cameraSensitivity;

  cameraYaw += xOffset;
  cameraPitch += yOffset;

  if (cameraPitch > 89.0f) cameraPitch = 89.0f;
  if (cameraPitch < -89.0f) cameraPitch = -89.0f;

  updateCameraVecs();
}

glm::mat4 getViewMatrix() {
  return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
