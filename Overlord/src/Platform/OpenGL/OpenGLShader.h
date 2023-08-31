#pragma once

#include "Overlord/Renderer/Shader.h"
#include <glm/glm.hpp>

// Temp
typedef unsigned int GLenum;

namespace Overlord
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Use() const override;
		virtual void Unuse() const override;

		// =================================================
		// Uniform update functions
		void SetInt		(const std::string& name, const int value) const;

		void SetFloat	(const std::string& name, const float value) const;
		void SetFloat2	(const std::string& name, const glm::vec2& vec2) const;
		void SetFloat3	(const std::string& name, const glm::vec3& vec3) const;
		void SetFloat4	(const std::string& name, const glm::vec4& vec4) const;

		void SetMat3	(const std::string& name, const glm::mat3& mat4) const;
		void SetMat4	(const std::string& name, const glm::mat4& mat4) const;
		// =================================================

	// Private Functions
	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string> shaderSource);

	// Private Data Members
	private:
		uint32_t m_ProgramID;
	};
}