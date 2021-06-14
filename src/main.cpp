#include <iostream>
#include <SDL2/SDL.h>
#include <glad/gl.h>

int main(void) {
    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Problem initialising SDL:" << std::endl;
        std::cerr << SDL_GetError() << std::endl;

        return -1;
    }

    // Create the window and renderer
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Catch if the renderer fails to be created
    if (renderer == nullptr) {
        std::cerr << "SDL renderer couldn't be created:" << std::endl;
        std::cerr << SDL_GetError() << std::endl;

        return -1;
    }

    // Create the context
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    // Load the OpenGL functions
    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
    if (version == 0) {
        std::cout << "Failed to initialise OpenGL" << std::endl;
        return -1;
    }

    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    SDL_Event event;
    bool running = true;
    while (running) {
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT) running = false;
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    // Teardown application
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}