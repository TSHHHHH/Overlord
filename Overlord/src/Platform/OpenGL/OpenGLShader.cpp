#include "oldpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Overlord
{
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			OLD_CORE_ERROR("{0}", infoLog.data());
			OLD_CORE_ASSERT(false, "Vertex shader compilation failure!!");

			return;
		}

		// ==================================================================

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			OLD_CORE_ERROR("{0}", infoLog.data());
			OLD_CORE_ASSERT(false, "Fragment shader compilation failure!!");

			return;
		}

		// ==================================================================

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_ProgramID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_ProgramID, vertexShader);
		glAttachShader(m_ProgramID, fragmentShader);

		// Link our program
		glLinkProgram(m_ProgramID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ProgramID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_ProgramID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			OLD_CORE_ERROR("{0}", infoLog.data());
			OLD_CORE_ASSERT(false, "Shader Program compilation failure!!");

			return;
		}

		// ==================================================================

		// Always detach shaders after a successful link.
		glDetachShader(m_ProgramID, vertexShader);
		glDetachShader(m_ProgramID, fragmentShader);
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
	// =================================================
}