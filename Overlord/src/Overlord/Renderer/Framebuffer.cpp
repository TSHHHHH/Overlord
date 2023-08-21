#include "oldpch.h"
#include "Framebuffer.h"

#include "Overlord/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Overlord
{
	Ref<Framebuffer> Overlord::Framebuffer::Create(const FramebufferSpec& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			OLD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLFramebuffer>(spec);
		}

		OLD_CORE_ASSERT(false, "Unknow Renderer API!!");
		return nullptr;
	}
}