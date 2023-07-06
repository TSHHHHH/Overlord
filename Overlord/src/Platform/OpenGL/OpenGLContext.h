#pragma once

#include "Overlord/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Overlord
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffer() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}