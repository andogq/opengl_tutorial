#include "Renderer.h"

void gl_clear_errors() {
    while (glGetError() != GL_NO_ERROR);
}

bool gl_check_errors(const char* function, const char* file, int line) {
    bool ok = true;

    while (unsigned int error = glGetError()) {
        std::cout << "[OpenGL Error] (0x" << std::hex << error << std::dec << "): " << file << ": " <<
            function << " (line "<<  line << ")" << std::endl;

        ok = false;
    }

    return ok;
}

void Renderer::draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const {
    shader.bind();
    vertex_array.bind();
    index_buffer.bind();

    gl_call(glDrawElements(GL_TRIANGLES, index_buffer.get_count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}