#include "gladospch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

//#include <shaderc/shaderc.hpp>
//#include <spirv_cross/spirv_cross.hpp>
//#include <spirv_cross/spirv_glsl.hpp>
//
//#include "Glados/Core/Timer.h"

namespace Glados {

	namespace Utils {

		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			GD_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}

		//static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		//{
		//	switch (stage)
		//	{
		//	case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
		//	case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		//	}
		//	GD_CORE_ASSERT(false);
		//	return (shaderc_shader_kind)0;
		//}

		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			GD_CORE_ASSERT(false, "");
			return nullptr;
		}

		//static const char* GetCacheDirectory()
		//{
		//	// TODO: make sure the assets directory is valid
		//	return "assets/cache/shader/opengl";
		//}

		//static void CreateCacheDirectoryIfNeeded()
		//{
		//	std::string cacheDirectory = GetCacheDirectory();
		//	if (!std::filesystem::exists(cacheDirectory))
		//		std::filesystem::create_directories(cacheDirectory);
		//}

		//static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		//{
		//	switch (stage)
		//	{
		//	case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
		//	case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
		//	}
		//	GD_CORE_ASSERT(false);
		//	return "";
		//}
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_FilePath(filepath)
	{

		//Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);

		//{
		//	Timer timer;
		//	CompileOrGetVulkanBinaries(shaderSources);
		//	CompileOrGetOpenGLBinaries();
		//	CreateProgram();
		//	GD_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		//}

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
		CreateProgram();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		//CompileOrGetVulkanBinaries(sources);
		//CompileOrGetOpenGLBinaries();
		CreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				GD_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			GD_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#shader";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			GD_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			GD_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			GD_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	ShaderIDs OpenGLShader::Compile(ShaderSources shaderSources)
	{
		int shaderIndex = 0;
		ShaderIDs shaderIDs;
		shaderIDs.reserve(shaderSources.size());

		for (auto shaderSource : shaderSources)
		{
			// create an empty shader object to hold shader strings
			GLenum type = shaderSource.first;
			const char* src = shaderSource.second.c_str();
			GLint id = glCreateShader(type);

			glShaderSource(id, 1, &src, nullptr); // add source code for shader

			glCompileShader(id); // compile the shader

			// get compilation status
			int result;
			glGetShaderiv(id, GL_COMPILE_STATUS, &result);

			if (result == GL_FALSE)
			{
				int length; // get number of characters in the info log
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // allocate memory on the stack for chars
				char* message = (char*)_malloca(length * sizeof(char)); // assign message memory
				glGetShaderInfoLog(id, length, &length, message); // get relevant data

				GD_CORE_ERROR("{0}({1}) failed to compile:\n{2}", m_Filepath, type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);

				for (int id : shaderIDs)
					glDeleteShader(id);

				shaderIDs.clear();
				return shaderIDs;
			}
			shaderIDs.push_back(id);
			shaderIndex++;
		}
		return shaderIDs;
	}

	GLint OpenGLShader::Link(ShaderIDs shaderIDs)
	{
		if (!shaderIDs.size())
			return 0;

		GLint program = glCreateProgram();

		// attaches shaders to the current program
		for (auto shaderID : shaderIDs)
			glAttachShader(program, shaderID);

		glLinkProgram(program);

		// get linking errors
		GLint result;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint length; // get number of characters in the info log
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length); // allocate memory on the stack for chars
			char* message = (char*)_malloca(length * sizeof(char)); // assign message memory
			glGetProgramInfoLog(program, length, &length, message); // get relevant data

			GD_CORE_ERROR("{0}: Failed to link program:\n{1}", m_Filepath, message);

			glDeleteProgram(program);
			return 0;
		}

		// delete shaders after linking
		for (auto shaderID : shaderIDs)
		{
			glDetachShader(program, shaderID);
			glDeleteShader(shaderID);
		}

		return program;
	}

	//void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	//{
	//	GLuint program = glCreateProgram();

	//	shaderc::Compiler compiler;
	//	shaderc::CompileOptions options;
	//	options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
	//	const bool optimize = true;
	//	if (optimize)
	//		options.SetOptimizationLevel(shaderc_optimization_level_performance);

	//	std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

	//	auto& shaderData = m_VulkanSPIRV;
	//	shaderData.clear();
	//	for (auto&& [stage, source] : shaderSources)
	//	{
	//		std::filesystem::path shaderFilePath = m_FilePath;
	//		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

	//		std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
	//		if (in.is_open())
	//		{
	//			in.seekg(0, std::ios::end);
	//			auto size = in.tellg();
	//			in.seekg(0, std::ios::beg);

	//			auto& data = shaderData[stage];
	//			data.resize(size / sizeof(uint32_t));
	//			in.read((char*)data.data(), size);
	//		}
	//		else
	//		{
	//			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
	//			if (module.GetCompilationStatus() != shaderc_compilation_status_success)
	//			{
	//				GD_CORE_ERROR(module.GetErrorMessage());
	//				GD_CORE_ASSERT(false);
	//			}

	//			shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

	//			std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
	//			if (out.is_open())
	//			{
	//				auto& data = shaderData[stage];
	//				out.write((char*)data.data(), data.size() * sizeof(uint32_t));
	//				out.flush();
	//				out.close();
	//			}
	//		}
	//	}

	//	for (auto&& [stage, data]: shaderData)
	//		Reflect(stage, data);
	//}

	//void OpenGLShader::CompileOrGetOpenGLBinaries()
	//{
	//	auto& shaderData = m_OpenGLSPIRV;

	//	shaderc::Compiler compiler;
	//	shaderc::CompileOptions options;
	//	options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
	//	const bool optimize = false;
	//	if (optimize)
	//		options.SetOptimizationLevel(shaderc_optimization_level_performance);

	//	std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

	//	shaderData.clear();
	//	m_OpenGLSourceCode.clear();
	//	for (auto&& [stage, spirv] : m_VulkanSPIRV)
	//	{
	//		std::filesystem::path shaderFilePath = m_FilePath;
	//		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

	//		std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
	//		if (in.is_open())
	//		{
	//			in.seekg(0, std::ios::end);
	//			auto size = in.tellg();
	//			in.seekg(0, std::ios::beg);

	//			auto& data = shaderData[stage];
	//			data.resize(size / sizeof(uint32_t));
	//			in.read((char*)data.data(), size);
	//		}
	//		else
	//		{
	//			spirv_cross::CompilerGLSL glslCompiler(spirv);
	//			m_OpenGLSourceCode[stage] = glslCompiler.compile();
	//			auto& source = m_OpenGLSourceCode[stage];

	//			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
	//			if (module.GetCompilationStatus() != shaderc_compilation_status_success)
	//			{
	//				GD_CORE_ERROR(module.GetErrorMessage());
	//				GD_CORE_ASSERT(false);
	//			}

	//			shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

	//			std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
	//			if (out.is_open())
	//			{
	//				auto& data = shaderData[stage];
	//				out.write((char*)data.data(), data.size() * sizeof(uint32_t));
	//				out.flush();
	//				out.close();
	//			}
	//		}
	//	}
	//}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			GD_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	//void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	//{
	//	spirv_cross::Compiler compiler(shaderData);
	//	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	//	GD_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
	//	GD_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
	//	GD_CORE_TRACE("    {0} resources", resources.sampled_images.size());

	//	GD_CORE_TRACE("Uniform buffers:");
	//	for (const auto& resource : resources.uniform_buffers)
	//	{
	//		const auto& bufferType = compiler.get_type(resource.base_type_id);
	//		uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
	//		uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
	//		int memberCount = bufferType.member_types.size();

	//		GD_CORE_TRACE("  {0}", resource.name);
	//		GD_CORE_TRACE("    Size = {0}", bufferSize);
	//		GD_CORE_TRACE("    Binding = {0}", binding);
	//		GD_CORE_TRACE("    Members = {0}", memberCount);
	//	}
	//}

	void OpenGLShader::Bind() const
	{
		

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		

		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		

		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}
