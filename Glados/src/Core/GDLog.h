#pragma once

#include "Core/Core.h"
#include <iostream>
#include <GL/glew.h>

#define LOG(x) std::cout << x << std::endl
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) Glados::GLClearError(); x;\
    ASSERT(Glados::GLLogCall(#x, __FILE__, __LINE__))

namespace Glados {

    // clears all active gl errors
    void GLADOS_API GLClearError();

    // calls glGetError() to find new errors
    bool GLADOS_API GLLogCall(const char* function, const char* file, unsigned int line);

    // prints all active uniforms
    void GLADOS_API GLPrintUniformInfo(unsigned int program);

}