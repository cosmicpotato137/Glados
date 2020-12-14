#include "gladospch.h"
#include "OpenGLShader.h"
#include "glad/glad.h"

namespace Glados {

	static UniformType GLTypeToGDUniformType(uint32_t type)
	{
		switch (type)
		{
		case GL_NONE:		return UniformType::None;
		case GL_FLOAT:		return UniformType::Float;
		case GL_FLOAT_VEC2:	return UniformType::Float2;
		case GL_FLOAT_VEC3:	return UniformType::Float3;
		case GL_FLOAT_VEC4:	return UniformType::Float4;
		case GL_FLOAT_MAT3:	return UniformType::Mat3;
		case GL_FLOAT_MAT4:	return UniformType::Mat4;
		case GL_INT:		return UniformType::Int;
		case GL_INT_VEC2:	return UniformType::Int2;
		case GL_INT_VEC3:	return UniformType::Int3;
		case GL_INT_VEC4:	return UniformType::Int4;
		case GL_BOOL:		return UniformType::Bool;
		case GL_SAMPLER_2D: return UniformType::Sampler2;
		}
        GD_CORE_ASSERT(false, "Unknown OpenGL type!");
        return UniformType::None;
	}

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

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_Filepath(filepath), m_BuildStatus(false), m_RendererID(0)
    {
		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
        BuildShader();
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment)
        : m_Name(name), m_BuildStatus(false), m_RendererID(0)
	{
        ShaderSources shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertex;
        shaderSources[GL_FRAGMENT_SHADER] = fragment;
		ShaderIDs shaderIDs = Compile(shaderSources);
		m_RendererID = Link(shaderIDs);

        m_BuildStatus = m_RendererID;
        if (m_BuildStatus)
		    LoadUniforms();
	}

	OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

	void OpenGLShader::BuildShader()
	{
        GD_CORE_VALIDATE(&m_Filepath, return, "{0}: Unable to load shader! No filepath specified.", m_Filepath);

        if (m_RendererID)
        {
            glDeleteProgram(m_RendererID);
            m_RendererID = 0;
        }

        std::string source = ReadFile(m_Filepath);
        ShaderSources shaderSources = Preprocess(source, "#shader");
        ShaderIDs shaderIDs = Compile(shaderSources);
        m_RendererID = Link(shaderIDs);

        m_BuildStatus = m_RendererID;
        if (m_BuildStatus)
		    m_UniformCache = LoadUniforms();
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = (size_t)in.tellg();
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

	ShaderSources OpenGLShader::Preprocess(const std::string& source, const std::string& typetoken)
	{
		ShaderSources shaderSources;

		const char* typeToken = typetoken.c_str();
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			GD_CORE_VALIDATE(eol != std::string::npos, return ShaderSources(), "{0}: syntax error", m_Filepath);
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			GD_CORE_VALIDATE(ShaderTypeFromString(type), return ShaderSources(), "{0}: Invalid shader type specified", m_Filepath);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			GD_CORE_VALIDATE(nextLinePos != std::string::npos, return ShaderSources(), "{0}: syntax error", m_Filepath);
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = 
                (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
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

	UniformMap OpenGLShader::LoadUniforms()
    {
        UniformMap uniformMap;

		// get uniforms
		int num_uniforms;
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &num_uniforms);
		char uniformName[256];
		int length;
		int size;
		unsigned int type;
		for (int i = 0; i < num_uniforms; i++)
		{
			glGetActiveUniform(m_RendererID, i, sizeof(uniformName), &length, &size, &type, uniformName);
            
            //TODO: make Uniform::Create method?
            if (m_UniformCache.Exists(uniformName))
            {
                m_UniformCache.GetUniform(uniformName)->SetID(glGetUniformLocation(m_RendererID, uniformName));
                uniformMap.AddUniform(m_UniformCache.GetUniform(uniformName));
            }
            else
            {
                Ref<Uniform> u = CreateRef<Uniform>(GLTypeToGDUniformType(type), uniformName);
                u->SetID(glGetUniformLocation(m_RendererID, uniformName));
                uniformMap.AddUniform(u);
            }

		}

        return uniformMap;
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

	void OpenGLShader::SetUniforms()
	{
        for (auto p : m_UniformCache)
        {
			Ref<Uniform> uniform = p.second;
			switch (uniform->Type)
			{
			case UniformType::Float:	glUniform1fv(uniform->GetID(), 1, (float*)uniform->GetData()); break;
			case UniformType::Float2:	glUniform2fv(uniform->GetID(), 1, (float*)uniform->GetData()); break;
			case UniformType::Float3:	glUniform3fv(uniform->GetID(), 1, (float*)uniform->GetData()); break;
			case UniformType::Float4:	glUniform4fv(uniform->GetID(), 1, (float*)uniform->GetData()); break;
			case UniformType::Mat3:		glUniformMatrix3fv(uniform->GetID(), 1, false, (float*)uniform->GetData()); break;
			case UniformType::Mat4:		glUniformMatrix4fv(uniform->GetID(), 1, false, (float*)uniform->GetData()); break;
			case UniformType::Int:		glUniform1iv(uniform->GetID(), 1, (int*)uniform->GetData()); break;
			case UniformType::Int2:		glUniform2iv(uniform->GetID(), 1, (int*)uniform->GetData()); break;
			case UniformType::Int3:		glUniform3iv(uniform->GetID(), 1, (int*)uniform->GetData()); break;
			case UniformType::Int4:		glUniform4iv(uniform->GetID(), 1, (int*)uniform->GetData()); break;
			case UniformType::Bool:		glUniform1iv(uniform->GetID(), 1, (int*)uniform->GetData()); break;
			}
        }
	}

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
		glUniform1i(m_UniformCache.GetUniform(name)->GetID(), value);
    }


    void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
		glUniform1f(m_UniformCache.GetUniform(name)->GetID(), value);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {

    }

	void OpenGLShader::SetFloat3(const std::string& name, const float* value)
	{
		glUniform3fv(m_UniformCache.GetUniform(name)->GetID(), 1, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        glUniform4fv(m_UniformCache.GetUniform(name)->GetID(), 1, &value[0]);
    }

	void OpenGLShader::SetFloat4(const std::string& name, const float* value)
	{
		glUniform4fv(m_UniformCache.GetUniform(name)->GetID(), 1, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
		glUniformMatrix4fv(m_UniformCache.GetUniform(name)->GetID(), 1, false, &value[0][0]);
    }

	template<UniformType T> void OpenGLShader::SetUniform(Ref<Uniform> uniform)
	{
		GD_CORE_WARN("Unknown uniform type");
	}

	template<>
	void OpenGLShader::SetUniform<UniformType::Bool>(const Ref<Uniform> uniform)
	{
		glUniform1iv(uniform->GetID(), 1, (int*)uniform->GetData());
	}
	
	template<>
	void OpenGLShader::SetUniform<UniformType::Int>(const Ref<Uniform> uniform)
	{
		glUniform1iv(uniform->GetID(), 1, (int*)uniform->GetData());
	}

	template<>
	void OpenGLShader::SetUniform<UniformType::Int2>(const Ref<Uniform> uniform)
	{
		glUniform2iv(uniform->GetID(), 1, (int*)uniform->GetData());
	}

	template<>
	void OpenGLShader::SetUniform<UniformType::Int3>(const Ref<Uniform> uniform)
	{
		glUniform3iv(uniform->GetID(), 1, (int*)uniform->GetData());
	}

	template<>
	void OpenGLShader::SetUniform<UniformType::Int4>(const Ref<Uniform> uniform)
	{
		glUniform4iv(uniform->GetID(), 1, (int*)uniform->GetData());
	}

	template<>
	void OpenGLShader::SetUniform<UniformType::Float>(const Ref<Uniform> uniform)
	{
		glUniform1fv(uniform->GetID(), 1, (float*)uniform->GetData());
	}

	template<>
	void OpenGLShader::SetUniform<UniformType::Float2>(const Ref<Uniform> uniform)
	{
		glUniform2fv(uniform->GetID(), 1, (float*)uniform->GetData());
	}

	template<>
	void OpenGLShader::SetUniform<UniformType::Float3>(const Ref<Uniform> uniform)
	{
		glUniform3fv(uniform->GetID(), 1, (float*)uniform->GetData());
	}

	template<>
	void OpenGLShader::SetUniform<UniformType::Float4>(const Ref<Uniform> uniform)
	{
		glUniform4fv(uniform->GetID(), 1, (float*)uniform->GetData());
	}

	template<>
	void OpenGLShader::SetUniform<UniformType::Mat3>(const Ref<Uniform> uniform)
	{
		glUniformMatrix3fv(uniform->GetID(), 1, false, (float*)uniform->GetData());
	}

	template<>
	void OpenGLShader::SetUniform<UniformType::Mat4>(const Ref<Uniform> uniform)
	{
		glUniformMatrix4fv(uniform->GetID(), 1, false, (float*)uniform->GetData());
	}

}