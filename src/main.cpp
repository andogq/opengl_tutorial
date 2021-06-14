#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <SDL2/SDL.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum ShaderType {
    ShaderType_Vertex,
    ShaderType_Fragment
};

unsigned int compile_shader(const std::string& name, ShaderType type) {
    // Create file stream
    std::string path = "res/shaders/" + name + "/" + (type == ShaderType_Vertex ? "vertex" : "fragment") + ".glsl";
    std::ifstream stream(path);

    // Read in file
    std::stringstream string_stream;
    string_stream << stream.rdbuf();

    // Convert to c string
    std::string source = string_stream.str();
    const char* raw_source = source.c_str();

    // Compile shader
    unsigned int shader = glCreateShader(type == ShaderType_Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &raw_source, nullptr);
    glCompileShader(shader);

    // Check status
    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        std::cerr << "There was a problem compiling the " << (type == ShaderType_Vertex ? "vertex" : "fragment") << " shader" << std::endl;

        int error_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_length);
        
        char* message = new char[error_length];
        glGetShaderInfoLog(shader, error_length, &error_length, message);

        std::cerr << message << std::endl;

        glDeleteShader(shader);
        return 0;
    } else return shader;
}

unsigned int create_shader(const std::string& name) {
    // Load the shaders from files
    unsigned int vertex_shader = compile_shader(name, ShaderType_Vertex);
    unsigned int fragment_shader = compile_shader(name, ShaderType_Fragment);

    // Create the program
    unsigned int program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

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

    // Create positions
    float positions[] = {
        -10.0f,  10.0f,
         10.0f,  10.0f,
         10.0f, -10.0f,
        -10.0f, -10.0f
    };
    // float positions[] = {
    //     -0.5f,  0.5f,
    //      0.5f,  0.5f,
    //      0.5f, -0.5f,
    //     -0.5f, -0.5f
    // };
    int indexes[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Create vertex buffer
    unsigned int vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    // Load vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);

    // Create attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    // Create index buffer
    unsigned int index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    // Load index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    // Load shaders
    unsigned int program = create_shader("basic");

    // Create uniform
    unsigned int u_mvp_matrix = glGetUniformLocation(program, "u_mvp_matrix");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Create mvp matrix
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 0.0f));
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.0f));
    glm::mat4 mvp = projection * view * model;

    SDL_Event event;
    bool running = true;
    while (running) {
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT) running = false;
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;

        glClear(GL_COLOR_BUFFER_BIT);


        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glUseProgram(program);
    
        glUniformMatrix4fv(u_mvp_matrix, 1, GL_FALSE, &mvp[0][0]);
    
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        SDL_GL_SwapWindow(window);
    }

    // Teardown application
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}