#include "oldpch.h"
#include "RendererAPI.h"

namespace Overlord
{
	// Default render API is OpenGL
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
}