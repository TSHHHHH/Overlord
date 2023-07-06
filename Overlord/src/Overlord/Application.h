#pragma once

#include "core.h"

#include "Window.h"

#include "Overlord/LayerStack.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Overlord/ImGui/ImGuiLayer.h"

#include "Overlord/Renderer/Shader.h"
#include "Overlord/Renderer/VertexArray.h"
#include "Overlord/Renderer/Buffer.h"

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

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& event);

		bool m_Running = true;

		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		LayerStack m_LayerStack;

		// Rendering Components
		std::shared_ptr<Shader>			m_Shader;
		std::shared_ptr<VertexArray>	m_VertexArray;

		std::shared_ptr<Shader>			m_Shader_Blue;
		std::shared_ptr<VertexArray>	m_VA_Square;
	};

	// To be defined in client
	Application* CreateApplication();
}
