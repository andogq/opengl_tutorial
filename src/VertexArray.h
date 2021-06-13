#pragma once

#include "error.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
    private:
        unsigned int renderer_id;

    public:
        VertexArray();
        ~VertexArray();

        void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

        void bind() const;
        void unbind() const;
};