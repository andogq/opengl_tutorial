#include <iostream>

#include "Renderer.h"

#define assert(x) if (!(x)) raise(SIGTRAP);

#ifdef DEBUG
#define gl_call(x) gl_clear_errors(); x; assert(gl_check_errors(#x, __FILE__, __LINE__))
#else
#define gl_call(x) x
#endif

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