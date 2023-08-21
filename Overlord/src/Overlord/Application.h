#pragma once

#include "core.h"

#include "Window.h"

#include "Overlord/LayerStack.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Overlord/ImGui/ImGuiLayer.h"

#include "Overlord/Core/Timestep.h"

namespace Overlord
{
	class Application
	{
	public:
		Application(const std::string& name = "Overlord Engine");
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }

		void ShutDownApplication();

	// private functions
	private:
		bool OnWindowClose(WindowCloseEvent& event);

	// private data members
	private:
		static Application* s_Instance;

		bool m_Running = true;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		LayerStack m_LayerStack;

		float m_LastFrameTime;
	};

	// To be defined in client
	Application* CreateApplication();
}
