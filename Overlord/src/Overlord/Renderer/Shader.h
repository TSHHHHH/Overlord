#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Overlord
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Use() const;
		void Unuse() const;

		void SetVec3(const std::string& name, const glm::vec3& vec3) const;

	private:
		uint32_t m_ProgramID;
	};
}