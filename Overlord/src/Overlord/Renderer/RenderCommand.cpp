#include "oldpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Overlord
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}