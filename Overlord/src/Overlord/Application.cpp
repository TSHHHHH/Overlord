#include "oldpch.h"
#include "Application.h"

#include "Log.h"

#include <glad/glad.h>

#include "Input.h"

// Temp
//#include <glm/glm.hpp>

namespace Overlord
{
	// Temp
	//glm::vec3 position;
	//glm::vec3 color;

	Application* Application::s_Instance = nullptr;

	// ==================================================================

	// For glVertexAttribPointer 3rd argument
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		OLD_CORE_ASSERT(false, "Unknow Shader Data Type!!");
		return 0;
	}

	// Definitions
	Application::Application()
	{
		OLD_CORE_ASSERT(!s_Instance, "Application already exists!!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(OLD_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// ==================================================================
		// Create Rendering Components
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
			// Vertex Pos		// Color
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};

		m_VertexBuffer->SetLayout(layout);

		// Set vertex attributes with for range loop
		uint32_t index = 0;
		for (const auto& element : m_VertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.isNormalized ? GL_TRUE : GL_FALSE, 
				m_VertexBuffer->GetLayout().GetStride(),
				(const void*)element.Offset);

			++index;
		}

		uint32_t indices[3] = { 0,1,2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			uniform vec3 position_byImGui;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;

				gl_Position = vec4(a_Position + position_byImGui, 1.0);
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
			glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Rendering
			m_Shader->Use();
			//m_Shader->SetVec3("position_byImGui", position);
			//m_Shader->SetVec3("color_byImGui", color);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			// Normal Layer system update
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// ImGui layer system update
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
}