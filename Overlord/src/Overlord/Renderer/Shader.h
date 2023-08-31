#pragma once

#include <string>

namespace Overlord
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Use() const = 0;
		virtual void Unuse() const = 0;

		static Shader* Create(const std::string& filePath);
		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

	private:
		uint32_t m_ProgramID;
	};
}