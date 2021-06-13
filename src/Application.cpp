#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define DEBUG

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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
        -0.5f, -0.5f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f, 1.0f, // 3
    };

    unsigned int indices[] {
        0, 1, 2,
        2, 3, 0
    };

    gl_call(glEnable(GL_BLEND));
    gl_call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray vertex_array;

    VertexBuffer vertex_buffer(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    vertex_array.add_buffer(vertex_buffer, layout);

    IndexBuffer index_buffer(indices, 6);

    Shader shader("res/shaders/basic.glsl");
    shader.bind();

    Texture texture("res/textures/art.png");
    texture.bind(0);
    shader.set_uniform_1i("u_texture", 0);

    vertex_array.unbind();
    vertex_buffer.unbind();
    index_buffer.unbind();
    shader.unbind();

    Renderer renderer;
    
    // Run until the window is closed
    while (!glfwWindowShouldClose(window)) {
        renderer.clear();

        // Call the draw function
        renderer.draw(vertex_array, index_buffer, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}