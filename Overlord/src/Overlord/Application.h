#pragma once

#include "core.h"

#include "Window.h"

#include "Overlord/LayerStack.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Overlord/ImGui/ImGuiLayer.h"

#include "Overlord/Renderer/Buffer.h"
#include "Overlord/Renderer/Shader.h"

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
		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	};

	// To be defined in client
	Application* CreateApplication();
}
