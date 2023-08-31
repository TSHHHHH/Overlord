#include "oldpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Overlord
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		OLD_CORE_ASSERT(false, "Unknown shader type!!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);

		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;

		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ProgramID);
	}

	void OpenGLShader::Use() const
	{
		glUseProgram(m_ProgramID);
	}

	void OpenGLShader::Unuse() const
	{
		glUseProgram(0);
	}

	// =================================================
	// Uniform update functions
	void OpenGLShader::SetInt(const std::string& name, const int value) const
	{
		GLint uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());

		glUniform1i(uniformLocation, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float value) const
	{
		GLint uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());

		glUniform1f(uniformLocation, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& vec2) const
	{
		GLint uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());

		glUniform2f(uniformLocation, vec2.x, vec2.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& vec3) const
	{
		GLint uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());

		glUniform3f(uniformLocation, vec3.x, vec3.y, vec3.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& vec4) const
	{
		GLint uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());

		glUniform4f(uniformLocation, vec4.x, vec4.y, vec4.z, vec4.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat3) const
	{
		GLint uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());

		glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat3));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat4) const
	{
		GLint uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());

		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat4));
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			OLD_CORE_ERROR("Could not open file '{0}'", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			OLD_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			OLD_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		// Create a program object.
		GLuint program = glCreateProgram();

		// Create a vector to store shader IDs
		std::vector<GLenum> glShaderIDs(shaderSources.size());

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			// Send the shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				OLD_CORE_ERROR("{0}", infoLog.data());
				OLD_CORE_ASSERT(false, "Shader compilation failure!!");

				return;
			}

			// Attach our shaders to our program
			glAttachShader(program, shader);

			glShaderIDs.push_back(shader);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			OLD_CORE_ERROR("{0}", infoLog.data());
			OLD_CORE_ASSERT(false, "Shader Program compilation failure!!");

			return;
		}

		// ==================================================================

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
		{
			glAttachShader(program, id);
		}

		m_ProgramID = program;
	}

	// =================================================
}