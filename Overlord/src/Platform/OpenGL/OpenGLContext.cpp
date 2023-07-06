#include "oldpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>

namespace Overlord
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		OLD_CORE_ASSERT(windowHandle, "Invalid window handle!!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		// glad init
		int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OLD_CORE_ASSERT(gladStatus, "Failed to initialize Glad!!");

		// Display OpenGL info in console
		OLD_CORE_INFO("OpenGL Info:");
		OLD_CORE_INFO("   Vendor:  {0}", (const char*)glGetString(GL_VENDOR));
		OLD_CORE_INFO("   Render:  {0}", (const char*)glGetString(GL_RENDERER));
		OLD_CORE_INFO("   Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}