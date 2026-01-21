#include <camera.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <constants.hpp>

float mouseSpeed = 4.0f;
float acceleration = 150.0f;
float damping = 0.2f;
float maxSpeed = 10.0f;
float fastCoef = 10.0f;
glm::vec2 mousePos = glm::vec2(0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f);
glm::quat cameraOrientation = glm::quat(glm::vec3(0.0f));
glm::vec3 moveSpeed = glm::vec3(0.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

void initCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
  cameraPos = pos;
  cameraOrientation = glm::lookAt(pos, target, up);
  worldUp = up;
}

void setUpVector(glm::vec3 up) {
  const auto view = getViewMatrix();
  const auto dir = -glm::vec3(view[0][2], view[1][2], view[2][2]);
  cameraOrientation = glm::lookAt(cameraPos, cameraPos + dir, up);
}

void updateCamera(double deltaTime, const float mouseX, const float mouseY, CameraMovement movement) {
  auto newMousePos = glm::vec2(mouseX, mouseY);
  auto delta = newMousePos - mousePos;
    auto deltaQuat =
        glm::quat(glm::vec3(mouseSpeed * delta.y, mouseSpeed * delta.x, 0.0f));
    cameraOrientation = glm::normalize(deltaQuat * cameraOrientation);
    setUpVector(worldUp);
    mousePos = newMousePos;

    const auto v = glm::mat4_cast(cameraOrientation);
    const auto forward = -glm::vec3(v[0][2], v[1][2], v[2][2]);
    const auto right = glm::vec3(v[0][0], v[1][0], v[2][0]);
    const auto up = glm::cross(right, forward);

    glm::vec3 accel(0.0f);
    if (movement.forward)
      accel += forward;
    if (movement.backward)
      accel -= forward;
    if (movement.left)
      accel -= right;
    if (movement.right)
      accel += right;
    if (movement.up)
      accel += up;
    if (movement.down)
      accel -= up;
    if (movement.fastSpeed)
      accel *= fastCoef;

    if (accel == glm::vec3(0.0f)) {
      moveSpeed -=
          moveSpeed *
          glm::min((1.0f / damping) * static_cast<float>(deltaTime), 1.0f);
    } else {
      moveSpeed += accel * acceleration * static_cast<float>(deltaTime);
      const float maximumSpeed = movement.fastSpeed ? maxSpeed * fastCoef : maxSpeed;
      if (glm::length(moveSpeed) > maximumSpeed)
        moveSpeed = glm::normalize(moveSpeed) * maximumSpeed;
    }
    cameraPos += moveSpeed * static_cast<float>(deltaTime);
    cameraPos.y = 0.0f;
}

glm::mat4 getViewMatrix() {
  const auto t = glm::translate(glm::mat4(1.0f), -cameraPos);
  const auto r = glm::mat4_cast(cameraOrientation);
  return r * t;
}
