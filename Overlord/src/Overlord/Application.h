#pragma once

#include "core.h"

#include "Window.h"

#include "Overlord/LayerStack.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Overlord
{
	class OLD_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& event);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack;
	};

	// To be defined in client
	Application* CreateApplication();
}
