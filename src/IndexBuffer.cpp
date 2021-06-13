#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count):
    count(count) {
    gl_call(glGenBuffers(1, &this->renderer_id));
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id));
    gl_call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    gl_call(glDeleteBuffers(1, &this->renderer_id));
}

void IndexBuffer::bind() const {
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id));
}

void IndexBuffer::unbind() const {
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::get_count() const {
    return this->count;
}