#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define DEBUG

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void) {
    GLFWwindow* window;

    // Initialise glfw
    if (!glfwInit()) return -1;

    // Use the core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Create the rendering context
    glfwMakeContextCurrent(window);

    // Lock framerate
    glfwSwapInterval(1);

    // Initialise glew
    if (glewInit() != GLEW_OK) {
        std::cout << "Problem initialising glew" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f, // 3
    };

    unsigned int indices[] {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray vertex_array;

    VertexBuffer vertex_buffer(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(2);
    vertex_array.add_buffer(vertex_buffer, layout);

    IndexBuffer index_buffer(indices, 6);

    Shader shader("res/shaders/basic.glsl");
    shader.bind();

    Renderer renderer;

    float r = 0.0f;
    float increment = 0.005f;
    
    // Run until the window is closed
    while (!glfwWindowShouldClose(window)) {
        renderer.clear();
        
        // Pulse the red channel
        r += increment;
        if (r > 1 || r < 0) increment = -increment;

        // Send the uniform
        shader.bind();
        shader.set_uniform_4f("u_color", r, 0.0f, 1.0f, 1.0f);

        // Call the draw function
        renderer.draw(vertex_array, index_buffer, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}