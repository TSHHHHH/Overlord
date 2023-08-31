#include "oldpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Overlord
{
	Shader* Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			OLD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLShader(filePath);
		}

		OLD_CORE_ASSERT(false, "Unknow Renderer API!!");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			OLD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		OLD_CORE_ASSERT(false, "Unknow Renderer API!!");
		return nullptr;
	}
}