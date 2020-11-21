#include "gladospch.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "glad/glad.h"

namespace Glados {

    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("GLADOS");
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
        GD_CORE_TRACE("Initialized Core Logger");
    }

    void GLClearError()
    {
        while (glGetError())
        {
            std::cout << "Clearing Errors" << std::endl;
        }
    }

    bool GLLogCall(const char* function, const char* file, unsigned int line)
    {
        while (GLenum error = glGetError())
        {
            std::cout << "[OpenGL Error] (" << error << ") in "
                << function << " " << file << ":" << line << std::endl;
            return false;
        }
        return true;
    }

    void GLPrintUniformInfo(unsigned int program)
    {
        int num_uniforms;
        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &num_uniforms);
        char uniform_name[256];
        int length;
        int size;
        unsigned int type;
        for (int i = 0; i < num_uniforms; i++)
        {
            glGetActiveUniform(program, i, sizeof(uniform_name), &length, &size, &type, uniform_name);
            std::cout << "Name: " << uniform_name << ", Length: " <<
                length << ", Size: " << size << ", Type: " << type << std::endl;
        }
    }

}