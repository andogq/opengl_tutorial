#pragma once

#include <vector>
#include <GL/glew.h>

#include "Renderer.h"

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalised;

    static unsigned int get_size_of_type(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return sizeof(GLfloat);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }

        assert(false);

        return 0;
    }
};

class VertexBufferLayout {
    private:
        std::vector<VertexBufferElement> elements;
        unsigned int stride;

    public:
        VertexBufferLayout():
            stride(0) {};

        template<typename T>
        void push(unsigned int count) {
            assert(false);
        }

        inline unsigned int get_stride() const {
            return this->stride;
        }

        inline std::vector<VertexBufferElement> get_elements() const {
            return this->elements;
        }
};

template<> inline void VertexBufferLayout::push<float>(unsigned int count) {
    this->elements.push_back({
        GL_FLOAT,
        count,
        GL_FALSE
    });

    this->stride += count * VertexBufferElement::get_size_of_type(GL_FLOAT);
}

template<> inline void VertexBufferLayout::push<unsigned int>(unsigned int count) {
    this->elements.push_back({
        GL_UNSIGNED_INT,
        count,
        GL_FALSE
    });

    this->stride += count * VertexBufferElement::get_size_of_type(GL_UNSIGNED_INT);
}

template<> inline void VertexBufferLayout::push<unsigned char>(unsigned int count) {
    this->elements.push_back({
        GL_UNSIGNED_BYTE,
        count,
        GL_TRUE
    });

    this->stride += count * VertexBufferElement::get_size_of_type(GL_UNSIGNED_BYTE);
}

