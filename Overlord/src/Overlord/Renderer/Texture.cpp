#include "oldpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Overlord
{
	Ref<Textur2D> Textur2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			OLD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}

		OLD_CORE_ASSERT(false, "Unknow Renderer API!!");
		return nullptr;
	}
}