#include "oldpch.h"
#include "Application.h"

#include "Overlord/Renderer/Renderer.h"

#include "Input.h"
#include "KeyCodes.h"

namespace Overlord
{
	Application* Application::s_Instance = nullptr;

	// ==================================================================
	// Definitions
	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		OLD_CORE_ASSERT(!s_Instance, "Application already exists!!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(OLD_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// ==================================================================
		// Create Rendering Components
		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			// Vertex Pos		// Color
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};
		std::shared_ptr<VertexBuffer> VB(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};
		VB->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(VB);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<IndexBuffer> IB(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(IB);

		// Try to render a square
		m_VA_Square.reset(VertexArray::Create());

		float vertices_square[3 * 4] = {
			// Vertex Pos
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> VB_Square(VertexBuffer::Create(vertices_square, sizeof(vertices_square)));

		VB_Square->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});
		m_VA_Square->AddVertexBuffer(VB_Square);

		uint32_t indices_square[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> m_IB_Square(IndexBuffer::Create(indices_square, sizeof(indices_square) / sizeof(uint32_t)));
		m_VA_Square->SetIndexBuffer(m_IB_Square);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			uniform vec3 position_byImGui;
			uniform mat4 u_ViewProjection;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;

				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string FragmentSrc = R"(
			#version 330 core

			in vec3 v_Position;
			in vec4 v_Color;

			layout(location = 0) out vec4 color;

			uniform vec3 color_byImGui;

			void main()
			{
				color = vec4(color_byImGui, 1.0);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, FragmentSrc));

		std::string vertexSrc_Blue = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string FragmentSrc_Blue = R"(
			#version 330 core

			in vec3 v_Position;

			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_Shader_Blue.reset(new Shader(vertexSrc_Blue, FragmentSrc_Blue));

		// ==================================================================
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
		dispatcher.Dispatch<KeyPressedEvent>(OLD_BIND_EVENT_FN(Application::OnInput));

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
			// Render a background color
			RenderCommand::SetClearColor({ 0.14f, 0.14f, 0.14f, 1.0f });
			RenderCommand::Clear();

			// Rendering
			m_Camera.SetRotation(m_roatation);

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_Shader_Blue, m_VA_Square);

			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

			// Normal Layer system update
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// ImGui layer system ImGui render
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnInput(KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == OVLD_KEY_W)
		{
			++m_roatation;
		}
		else if (event.GetKeyCode() == OVLD_KEY_S)
		{
			--m_roatation;
		}

		return true;
	}
}