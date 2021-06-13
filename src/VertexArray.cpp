#include "VertexArray.h"

VertexArray::VertexArray() {
    gl_call(glGenVertexArrays(1, &this->renderer_id));
}
VertexArray::~VertexArray() {
    gl_call(glDeleteVertexArrays(1, &this->renderer_id));
}

void VertexArray::add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    this->bind();

    vb.bind();

    const std::vector<VertexBufferElement> elements = layout.get_elements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++) {
        const VertexBufferElement element = elements[i];

        // Define the layout of the buffer and enable it
        glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.get_stride(), (const void*) offset);
        glEnableVertexAttribArray(i);

        offset += element.count * VertexBufferElement::get_size_of_type(element.type);
    }
}

void VertexArray::bind() const {
    gl_call(glBindVertexArray(this->renderer_id));
}

void VertexArray::unbind() const {
    gl_call(glBindVertexArray(0));
}
