#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>

#define DEBUG

#define assert(x) if (!(x)) raise(SIGTRAP);

#ifdef DEBUG
#define gl_call(x) gl_clear_errors(); x; assert(gl_check_errors(#x, __FILE__, __LINE__))
#else
#define gl_call(x) x
#endif

static void gl_clear_errors() {
    while (glGetError() != GL_NO_ERROR);
}

static bool gl_check_errors(const char* function, const char* file, int line) {
    bool ok = true;

    while (unsigned int error = glGetError()) {
        std::cout << "[OpenGL Error] (0x" << std::hex << error << std::dec << "): " << file << ": " <<
            function << " (line "<<  line << ")" << std::endl;

        ok = false;
    }

    return ok;
}

struct ShaderProgramSource {
    std::string vertex_source;
    std::string fragment_source;
};

static ShaderProgramSource parse_shader(const std::string& filePath) {
    std::ifstream stream(filePath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos) type = ShaderType::FRAGMENT;
        } else ss[(int)type] << line << std::endl;
    }

    return {
        ss[0].str(),
        ss[1].str()
    };
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
    // Create the shader
    unsigned int id = glCreateShader(type);

    // Convert source to raw string
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Check the status of the compilation
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = new char[length];

        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "shader") << " shader" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static int create_shader(const std::string& vertexShader, const std::string& fragmentShader) {
    // Create a program
    unsigned int program_id = glCreateProgram();

    // Compile the shaders
    unsigned int vs_id = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs_id = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach the shader to the program
    glAttachShader(program_id, vs_id);
    glAttachShader(program_id, fs_id);

    // Link and validate the program
    glLinkProgram(program_id);
    glValidateProgram(program_id);

    // Delete the intermediate shaders
    glDeleteShader(vs_id);
    glDeleteShader(fs_id);

    return program_id;
}

int main(void) {
    GLFWwindow* window;

    // Initialise glfw
    if (!glfwInit()) return -1;

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

    // Generate the vertex buffer
    unsigned int buffer_id;
    glGenBuffers(1, &buffer_id);

    // Select the buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

    // Add data to the buffer
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

    // Define the layout of the buffer and enable it
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Generate the index buffer
    unsigned int index_buffer_id;
    glGenBuffers(1, &index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    ShaderProgramSource source = parse_shader("res/shaders/basic.glsl");

    unsigned int shader_id = create_shader(source.vertex_source, source.fragment_source);
    glUseProgram(shader_id);

    // Send color information into the shader
    int uniform_location = glGetUniformLocation(shader_id, "u_color");
    assert(uniform_location != -1);

    float r = 0.0f;
    float increment = 0.005f;
    
    // Run until the window is closed
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Send the uniform
        glUniform4f(uniform_location, r, 0.0f, 1.0f, 1.0f);

        // Pulse the red channel
        r += increment;
        if (r > 1 || r < 0) increment = -increment;

        gl_call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader_id);

    glfwTerminate();
    return 0;
}