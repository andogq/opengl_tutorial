#pragma once

#include <GL/glew.h>
#include <signal.h>

#define assert(x) if (!(x)) raise(SIGTRAP);

#ifdef DEBUG
#define gl_call(x) gl_clear_errors(); x; assert(gl_check_errors(#x, __FILE__, __LINE__))
#else
#define gl_call(x) x
#endif

void gl_clear_errors();

bool gl_check_errors(const char* function, const char* file, int line);