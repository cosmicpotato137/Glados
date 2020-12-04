#include "gladospch.h"
#include "OpenGLShader.h"
#include "glad/glad.h"

namespace Glados {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		GD_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

    static std::string ShaderTypeToString(GLenum type)
    {
        switch (type)
        {
        case GL_VERTEX_SHADER:
            return "vertex";
        case GL_FRAGMENT_SHADER:
            return "fragment";
        }

        GD_CORE_ASSERT(false, "Unknown shader enum");
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath)
        : m_Name(name), m_Filepath(filepath), m_RendererID(0)
    {
        std::string source = ReadFile(filepath);
        m_ShaderSources = Preprocess(source, "#shader");
        Compile();
        m_RendererID = Link();
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

	const std::string& OpenGLShader::GetName() const
	{
        return m_Name;
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

	std::unordered_map<GLenum, ShaderSource> OpenGLShader::Preprocess(const std::string& source, const std::string& typetoken)
	{
		std::unordered_map<GLenum, ShaderSource> shaderSources;

		const char* typeToken = typetoken.c_str();
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			GD_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			GD_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			GD_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

            ShaderSource shaderSource;
            shaderSource.Source = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = shaderSource;
		}

		return shaderSources;
	}

	void OpenGLShader::Compile()
	{
        for (auto it = m_ShaderSources.begin(); it != m_ShaderSources.end(); ++it)
        {
		    // create an empty shader object to hold shader strings
            GLenum type = it->first;
		    const char* src = it->second.Source.c_str();
            GLint id = glCreateShader(type);
            it->second.ID = id;

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

                GD_CORE_ERROR("Failed to compile {0} shader!", type == GL_VERTEX_SHADER ? "vertex" : "fragment");

			    glDeleteShader(id);
		    }
        }
	}

	uint32_t OpenGLShader::Link()
	{
		uint32_t program = glCreateProgram();

		// attaches shaders to the current program
        for (auto shaderSource : m_ShaderSources)
        {
            glAttachShader(program, shaderSource.second.ID);
			glDeleteShader(shaderSource.second.ID);
        }

        glLinkProgram(program);
        int result;
        glGetProgramiv(program, GL_LINK_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length; // get number of characters in the info log
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length); // allocate memory on the stack for chars
			char* message = (char*)_malloca(length * sizeof(char)); // assign message memory
			glGetProgramInfoLog(program, length, &length, message); // get relevant data

			GD_CORE_ERROR("Failed to link program: '{0}'", m_Name);

			glDeleteProgram(program);
            return 0;
		}

        return program;
	}

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    // sets the binding point of a uniform block and returns its index
    void OpenGLShader::SetUniformBlockIndex(const std::string& name, unsigned int binding)
    {
        unsigned int ubo = glGetUniformBlockIndex(m_RendererID, name.c_str());
        glUniformBlockBinding(m_RendererID, ubo, binding);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
    }


    void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
    
    }
    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        glUniform4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, false, &value[0][0]);
    }

    void OpenGLShader::SetUniform4f(const std::string& name,
        float v0, float v1, float v2, float v3)
    {
        glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
    }

    void OpenGLShader::SetUniform4fv(const std::string& name, const float* value)
    {
        glUniform4fv(GetUniformLocation(name), 1, value);
    }

    void OpenGLShader::SetUniform3f(const std::string& name,
        float v0, float v1, float v2)
    {
        glUniform3f(GetUniformLocation(name), v0, v1, v2);
    }

    void OpenGLShader::SetUniform3fv(const std::string& name, const float* value)
    {
        glUniform3fv(GetUniformLocation(name), 1, value);
    }

    void OpenGLShader::SetUniformMat4fv(const std::string& name, bool transpose, const float* value)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, value);
    }

    void OpenGLShader::SetUniform1f(const std::string& name, float f)
    {
        glUniform1f(GetUniformLocation(name), f);
    }

    void OpenGLShader::SetUniform1i(const std::string& name, int i)
    {
        glUniform1i(GetUniformLocation(name), i);
    }

    void OpenGLShader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
    }

    int OpenGLShader::GetUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        int location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
            std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
            m_UniformLocationCache[name] = location;
        }
        //std::cout << location << std::endl;
        return location;
    }

#ifdef _DEBUG
    void OpenGLShader::PrintUniforms()
    {
        GLPrintUniformInfo(m_RendererID);
    }
#else
    void OpenGLShader::PrintUniforms() {}
#endif

}