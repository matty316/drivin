#include <buffer.hpp>

GLuint createBuffers(std::span<Vertex> vertices, std::span<uint32_t> indices) {
  GLuint vao, vbo, ibo;

  glCreateBuffers(1, &vbo);
  glNamedBufferStorage(vbo, sizeof(Vertex)*vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &ibo);
  glNamedBufferStorage(ibo, sizeof(uint32_t)*indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateVertexArrays(1, &vao);

  glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
  glVertexArrayElementBuffer(vao, ibo);

  glEnableVertexArrayAttrib(vao, 0);
  glEnableVertexArrayAttrib(vao, 1);
  glEnableVertexArrayAttrib(vao, 2);

  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
  glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, norm));
  glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));

  glVertexArrayAttribBinding(vao, 0, 0);
  glVertexArrayAttribBinding(vao, 1, 0);
  glVertexArrayAttribBinding(vao, 2, 0);

  return vao;
}
