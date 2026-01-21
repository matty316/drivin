#include <game.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <print>
#include <shader.hpp>
#include <buffer.hpp>
#include <vertex.hpp>
#include <texture.hpp>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <camera.hpp>
#include <constants.hpp>

SDL_Window *window;
SDL_GLContext context;
GLuint program, vao, texture;
Uint64 last = SDL_GetPerformanceCounter();
double deltaTime = 0.0;
CameraMovement movement;
struct MouseState { float x, y; } mouseMovement;

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

  if (!SDL_SetWindowRelativeMouseMode(window, true)) exit(EXIT_FAILURE);

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
  initCamera({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
}

bool processKeyDown(SDL_Event event) {
  if (event.key.key == SDLK_ESCAPE)
    return true;
  return false;
}

void update() {
  auto now = SDL_GetPerformanceCounter();
  deltaTime = ((now - last) / (double)SDL_GetPerformanceFrequency());
  last = now;

  const auto *keyStates = SDL_GetKeyboardState(nullptr);
  movement.forward = keyStates[SDL_SCANCODE_W] == 1;
  movement.backward = keyStates[SDL_SCANCODE_S] == 1;
  movement.left = keyStates[SDL_SCANCODE_A] == 1;
  movement.right = keyStates[SDL_SCANCODE_D] == 1;

  float x, y;
  const auto mouseState = SDL_GetRelativeMouseState(&x, &y);
  mouseMovement.x += x;
  mouseMovement.y += y;
  std::println("({}, {})", x, y);
  int width, height;
  SDL_GetWindowSize(window, &width, &height);

  updateCamera(deltaTime, mouseMovement.x / (float)width, mouseMovement.y / (float)height, movement);
}

void draw() {
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  glViewport(0, 0, width, height);
  glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);

  auto proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
  setMat4(program, "proj", proj);

  setMat4(program, "view", getViewMatrix());

  auto model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  setMat4(program, "model", model);

  glBindTextureUnit(0, texture);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  SDL_GL_SwapWindow(window);
}

void run() {
  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) done = true;
      if (event.type == SDL_EVENT_KEY_DOWN) done = processKeyDown(event);
      // if (event.type == SDL_EVENT_MOUSE_MOTION) {
      //   mouseMovement.x += event.motion.xrel;
      //   mouseMovement.y += event.motion.yrel;
      // }
    }
    update();
    draw();
  }
}

void cleanup() {
  glDeleteProgram(program);
  glDeleteVertexArrays(1, &vao);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
