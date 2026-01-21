#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

GLuint createProgram(std::string vertPath, std::string fragPath);
void setMat4(GLuint program, const std::string &name, const glm::mat4 &mat);
