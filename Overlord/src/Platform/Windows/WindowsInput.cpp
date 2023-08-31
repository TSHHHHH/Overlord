#include "oldpch.h"
#include "Overlord/Input.h"

#include "Overlord/Application.h"
#include <GLFW/glfw3.h>

namespace Overlord
{
	bool Input::IsKeyPressed(int keycode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		int state = glfwGetKey(window, keycode);

		return (state == GLFW_PRESS) || (state == GLFW_REPEAT);
	}

	bool Input::IMouseButtonPressed(int mouseButton)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		int state = glfwGetKey(window, mouseButton);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}