#include "oldpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "Overlord/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Overlord
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		// ImGui set up
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Temp key map
		io.KeyMap[ImGuiKey_Tab]			= GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow]	= GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow]	= GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow]		= GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow]	= GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp]		= GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown]	= GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home]		= GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End]			= GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert]		= GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete]		= GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace]	= GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space]		= GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter]		= GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape]		= GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A]			= GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C]			= GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V]			= GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X]			= GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y]			= GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z]			= GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.f ? (time - m_Time) : (1.f / 60.f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// Display demo window
		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(OLD_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(OLD_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<MouseButtonPressedEvent>(OLD_BIND_EVENT_FN(ImGuiLayer::OnMousePressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(OLD_BIND_EVENT_FN(ImGuiLayer::OnMouseReleasedEvent));
		dispatcher.Dispatch<KeyPressedEvent>(OLD_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(OLD_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(OLD_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(OLD_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	// Input event definitions
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(event.GetX(), event.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += event.GetXoffset();
		io.MouseWheel += event.GetYoffset();

		return false;
	}

	bool ImGuiLayer::OnMousePressedEvent(MouseButtonPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseReleasedEvent(MouseButtonReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();

		int keyCode = event.GetKeyCode();
		if (keyCode > 0 && keyCode < 0x100000)
		{
			io.AddInputCharacter((unsigned short)keyCode);
		}

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)event.GetWidth(), (float)event.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.f, 1.f);
		glViewport(0, 0, event.GetWidth(), event.GetHeight());

		return false;
	}
}