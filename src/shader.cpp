#include <shader.hpp>
#include <fstream>
#include <sstream>
#include <print>

GLuint compileShader(const std::string &path, GLint shaderType) {
  std::ifstream f(path);

  if (!f.is_open()) {
    std::print("unable to open file: {}", path);
    exit(EXIT_FAILURE);
  }

  std::stringstream s;
  const char* code;

  s << f.rdbuf();
  f.close();

  auto codeStr = s.str();
  code = codeStr.c_str();

  auto shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &code, nullptr);
  glCompileShader(shader);

  int success;
  char infoLog[1024];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::println("unable to compile shader: {} | {}", path, infoLog);
      exit(EXIT_FAILURE);
  }
  return shader;
}

GLuint createProgram(const std::string &vertPath, const std::string &fragPath) {
  auto vert = compileShader(vertPath, GL_VERTEX_SHADER);
  auto frag = compileShader(fragPath, GL_FRAGMENT_SHADER);

  auto program = glCreateProgram();
  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);

  glDeleteShader(vert);
  glDeleteShader(frag);

  int success;
  char infoLog[1024];

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
      glGetProgramInfoLog(program, 1024, NULL, infoLog);
      std::println("unable to link program: {}", infoLog);
      exit(EXIT_FAILURE);
  }

  return program;
}

void setMat4(GLuint program, const std::string &name, const glm::mat4 &mat) {
  glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
