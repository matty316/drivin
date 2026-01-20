#version 460 core

in vec2 tex;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main() {
  FragColor = texture(textureSampler, tex);
}
