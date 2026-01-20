#include <game.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <print>
#include <shader.hpp>
#include <buffer.hpp>
#include <vertex.hpp>

SDL_Window *window;
SDL_GLContext context;
constexpr int WIDTH = 1920, HEIGHT = 1080;
GLuint program, vao;

void init() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

  window = SDL_CreateWindow("drivin", WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (window == NULL) {
    // In the case that the window could not be made...
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    std::print("could not create SDL window");
    exit(EXIT_FAILURE);
  }

  context = SDL_GL_CreateContext(window);

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::print("could not load glad");
    exit(EXIT_FAILURE);
  }
  program = createProgram("shaders/shader.vert", "shaders/shader.frag");

  //TEMP
  Vertex vertices[] = {
    {.pos = { 0.5f,  0.5f, 0.0f}, .norm = {0.0f, 1.0f, 0.0f}, .tex = {0.0f, 1.0f}},
    {.pos = { 0.5f, -0.5f, 0.0f}, .norm = {0.0f, 1.0f, 0.0f}, .tex = {0.0f, 1.0f}},
    {.pos = {-0.5f, -0.5f, 0.0f}, .norm = {0.0f, 1.0f, 0.0f}, .tex = {0.0f, 1.0f}},
    {.pos = {-0.5f,  0.5f, 0.0f}, .norm = {0.0f, 1.0f, 0.0f}, .tex = {0.0f, 1.0f}},
  };

  uint32_t indices[] = {0, 1, 3, 1, 2, 3};

  vao = createBuffers(vertices, indices);
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
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
  }
}

void cleanup() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}
