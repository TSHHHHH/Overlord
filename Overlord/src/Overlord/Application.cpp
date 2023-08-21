#include "oldpch.h"
#include "Application.h"

#include "Overlord/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Overlord
{
	Application* Application::s_Instance = nullptr;

	// ==================================================================
	// Definitions
	Application::Application(const std::string& name)
	{
		OLD_CORE_ASSERT(!s_Instance, "Application already exists!!");
		s_Instance = this;

		// Window Init
		m_Window = std::unique_ptr<Window>(Window::Create(name));
		m_Window->SetEventCallback(OLD_BIND_EVENT_FN(Application::OnEvent));

		// Renderer Init
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		// Dispatch window close event to trigger close function
		dispatcher.Dispatch<WindowCloseEvent>(OLD_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.isHandled())
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Normal Layer system update
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			// ImGui layer system ImGui render
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::ShutDownApplication()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		ShutDownApplication();
		return true;
	}
}