#include <game.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <stdexcept>

SDL_Window *window;
SDL_GLContext context;
constexpr int WIDTH = 1920, HEIGHT = 1080;

void init() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

  window = SDL_CreateWindow("drivin", WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (window == NULL) {
    // In the case that the window could not be made...
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    std::runtime_error("could not create SDL window");
  }

  context = SDL_GL_CreateContext(window);

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::runtime_error("could not load glad");
  }
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

    SDL_GL_SwapWindow(window);
  }
}

void cleanup() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}
