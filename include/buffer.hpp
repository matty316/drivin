#pragma once

#include <glad/glad.h>
#include <vertex.hpp>
#include <span>

GLuint createBuffers(std::span<Vertex> vertices, std::span<uint32_t> indices);
