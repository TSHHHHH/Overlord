#include "oldpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Overlord
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
				OLD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return new OpenGLVertexArray();
		}

		OLD_CORE_ASSERT(false, "Unknow Renderer API!!");
		return nullptr;
	}
}