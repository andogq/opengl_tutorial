#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    gl_call(glGenBuffers(1, &this->renderer_id));
    gl_call(glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id));
    gl_call(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    gl_call(glDeleteBuffers(1, &this->renderer_id));
}

void VertexBuffer::bind() const {
    gl_call(glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id));
}

void VertexBuffer::unbind() const {
    gl_call(glBindBuffer(GL_ARRAY_BUFFER, 0));
}