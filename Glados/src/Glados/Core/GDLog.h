#pragma once
#include "Glados/Core/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) Glados::GLClearError(); x;\
    ASSERT(Glados::GLLogCall(#x, __FILE__, __LINE__))

namespace Glados {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

    // clears all active gl errors
    void GLClearError();

    // calls glGetError() to find new errors
    bool GLLogCall(const char* function, const char* file, unsigned int line);

    // prints all active uniforms
    void GLPrintUniformInfo(unsigned int program);

}

#ifndef GD_DIST
// core Log macros
#define GD_CORE_TRACE(...)	::Glados::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GD_CORE_INFO(...)	::Glados::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GD_CORE_WARN(...)	::Glados::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GD_CORE_ERROR(...)	::Glados::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GD_CORE_FATAL(...)	::Glados::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// client Log macros
#define GD_FATAL(...)		::Glados::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define GD_TRACE(...)		::Glados::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GD_INFO(...)		::Glados::Log::GetClientLogger()->info(__VA_ARGS__)
#define GD_WARN(...)		::Glados::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GD_ERROR(...)		::Glados::Log::GetClientLogger()->error(__VA_ARGS__)
#define GD_FATAL(...)		::Glados::Log::GetClientLogger()->fatal(__VA_ARGS__)

#else
#define GD_CORE_TRACE(...)
#define GD_CORE_INFO(...)
#define GD_CORE_WARN(...)
#define GD_CORE_ERROR(...)
#define GD_CORE_FATAL(...)

#define GD_TRACE(...)	
#define GD_INFO(...)	
#define GD_WARN(...)	
#define GD_ERROR(...)	
#endif