#include "oldpch.h"
#include "WindowsInput.h"

#include "Overlord/Application.h"
#include <GLFW/glfw3.h>

namespace Overlord
{
	Input* Input::s_Instance = new WindowsInput();


	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		int state = glfwGetKey(window, keycode);

		return state == (GLFW_PRESS || GLFW_REPEAT);
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int mouseButton)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		int state = glfwGetKey(window, mouseButton);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePostionImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePostionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePostionImpl();
		return y;
	}
}