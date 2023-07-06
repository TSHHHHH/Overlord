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
			case RendererAPI::None:
				OLD_CORE_ASSERT(false, "RendererAPI::None is currently not supported!!");
				return nullptr;

			case RendererAPI::OpenGL:
				return new OpenGLVertexArray();
		}

		OLD_CORE_ASSERT(false, "Unknow Renderer API!!");
		return nullptr;
	}
}