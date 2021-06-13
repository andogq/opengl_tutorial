#include "Shader.h"

Shader::Shader(const std::string& file_path):
    file_path(file_path) {
    ShaderProgramSource source = this->parse_shader(file_path);
    this->renderer_id = this->create_shader(source.vertex_source, source.fragment_source);
}

Shader::~Shader() {
    gl_call(glDeleteProgram(this->renderer_id));
}

void Shader::bind() const {
    gl_call(glUseProgram(this->renderer_id));
}

void Shader::unbind() const {
    gl_call(glUseProgram(0));
}

void Shader::set_uniform_1i(const std::string& name, int value) {
    int location = this->get_uniform_location(name);

    gl_call(glUniform1i(location, value));
}

void Shader::set_uniform_1f(const std::string& name, float value) {
    int location = this->get_uniform_location(name);

    gl_call(glUniform1f(location, value));
}

void Shader::set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3) {
    int location = this->get_uniform_location(name);

    gl_call(glUniform4f(location, v0, v1, v2, v3));
}

ShaderProgramSource Shader::parse_shader(const std::string& file_path) {
    std::ifstream stream(this->file_path);

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

unsigned int Shader::compile_shader(unsigned int type, const std::string& source) {
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

unsigned int Shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader) {
    // Create a program
    unsigned int program_id = glCreateProgram();

    // Compile the shaders
    unsigned int vs_id = this->compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs_id = this->compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

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

int Shader::get_uniform_location(const std::string& name) {
    if (this->uniform_location_cache.find(name) != this->uniform_location_cache.end()) return this->uniform_location_cache[name];

    int location = glGetUniformLocation(this->renderer_id, name.c_str());
    this->uniform_location_cache[name] = location;

    if (location == -1) std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;

    return location;
}
