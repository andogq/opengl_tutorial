#pragma once

#include <signal.h>

#define _assert(x) if (!(x)) raise(SIGTRAP);

#ifdef DEBUG
#define gl_call(x) gl_clear_errors(); x; _assert(gl_check_errors(#x, __FILE__, __LINE__))
#else
#define gl_call(x) x
#endif