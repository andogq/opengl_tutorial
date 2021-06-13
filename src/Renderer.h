#pragma once

#include <GL/glew.h>
#include <iostream>

#include "error.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

void gl_clear_errors();

bool gl_check_errors(const char* function, const char* file, int line);

class Renderer {
    public:
        void draw(const VertexArray& vb, const IndexBuffer& ib, const Shader& shader) const;
        void clear() const;
};