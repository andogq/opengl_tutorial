#pragma once

#include <GL/glew.h>

#include "error.h"
class IndexBuffer {
    private:
        unsigned int renderer_id;
        unsigned int count;

    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void bind() const;
        void unbind() const;

        unsigned int get_count() const;
};