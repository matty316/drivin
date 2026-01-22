#include <texture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <print>

GLuint createTexture(const std::string &path) {
  int width, height, nrChannels;
  auto *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  if (!data) {
    std::println("unable to load image, {}", path);
    exit(EXIT_FAILURE);
  }

  GLuint texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);

  glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureStorage2D(texture, 1, GL_RGB8, width, height);
  glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

  glGenerateTextureMipmap(texture);

  stbi_image_free(data);

  return texture;
}
