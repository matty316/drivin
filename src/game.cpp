#include <game.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <print>
#include <shader.hpp>
#include <buffer.hpp>
#include <vertex.hpp>
#include <texture.hpp>
#include <stb_image.h>

SDL_Window *window;
SDL_GLContext context;
constexpr int WIDTH = 1920, HEIGHT = 1080;
GLuint program, vao, texture;

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
	}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
	}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
	}();
	std::println("{}, {}, {}, {}: {}", src_str, type_str, severity_str, id, message);
}

void init() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

  window = SDL_CreateWindow("drivin", WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (window == NULL) {
    // In the case that the window could not be made...
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    std::println("could not create SDL window");
    exit(EXIT_FAILURE);
  }

  context = SDL_GL_CreateContext(window);

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::println("could not load glad");
    exit(EXIT_FAILURE);
  }

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(message_callback, nullptr);

  program = createProgram("shaders/shader.vert", "shaders/shader.frag");

  //TEMP
  Vertex vertices[] = {
    {.pos = { 0.5f,  0.5f, 0.0f}, .norm = {1.0f, 0.0f, 0.0f}, .tex = {1.0f, 1.0f}},
    {.pos = { 0.5f, -0.5f, 0.0f}, .norm = {1.0f, 0.0f, 0.0f}, .tex = {1.0f, 0.0f}},
    {.pos = {-0.5f, -0.5f, 0.0f}, .norm = {1.0f, 0.0f, 0.0f}, .tex = {0.0f, 0.0f}}, 
    {.pos = {-0.5f,  0.5f, 0.0f}, .norm = {1.0f, 0.0f, 0.0f}, .tex = {0.0f, 1.0f}},
  };

  uint32_t indices[] = {0, 1, 3, 1, 2, 3};

  vao = createBuffers(vertices, indices);

  stbi_set_flip_vertically_on_load(true);
  texture = createTexture("textures/container.jpg");
}

void run() {
  bool done = false;
  while (!done) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
          done = true;
      }
    }

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindTextureUnit(0, texture);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
  }
}

void cleanup() {
  glDeleteProgram(program);
  glDeleteVertexArrays(1, &vao);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
