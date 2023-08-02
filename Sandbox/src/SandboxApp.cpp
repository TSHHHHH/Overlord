#include <Overlord.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Overlord::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_SquarePosition(0.f)
	{
		// ==================================================================
		// Create Rendering Components
		m_VertexArray.reset(Overlord::VertexArray::Create());

		float vertices[3 * 7] = {
			// Vertex Pos		// Color
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};
		std::shared_ptr<Overlord::VertexBuffer> VB(Overlord::VertexBuffer::Create(vertices, sizeof(vertices)));

		Overlord::BufferLayout layout = {
			{ Overlord::ShaderDataType::Float3, "a_Position" },
			{ Overlord::ShaderDataType::Float4, "a_Color" },
		};
		VB->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(VB);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<Overlord::IndexBuffer> IB(Overlord::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(IB);

		// Try to render a square
		m_VA_Square.reset(Overlord::VertexArray::Create());

		float vertices_square[3 * 4] = {
			// Vertex Pos
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Overlord::VertexBuffer> VB_Square(Overlord::VertexBuffer::Create(vertices_square, sizeof(vertices_square)));

		VB_Square->SetLayout({
			{ Overlord::ShaderDataType::Float3, "a_Position" }
			});
		m_VA_Square->AddVertexBuffer(VB_Square);

		uint32_t indices_square[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Overlord::IndexBuffer> m_IB_Square(Overlord::IndexBuffer::Create(indices_square, sizeof(indices_square) / sizeof(uint32_t)));
		m_VA_Square->SetIndexBuffer(m_IB_Square);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;

				gl_Position = u_ViewProjection *  u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string FragmentSrcFlat = R"(
			#version 330 core

			in vec3 v_Position;

			uniform vec4 u_Color;

			layout(location = 0) out vec4 color;

			void main()
			{
				color = u_Color;
			}
		)";

		//std::string FragmentSrc = R"(
		//	#version 330 core

		//	in vec3 v_Position;
		//	in vec4 v_Color;

		//	layout(location = 0) out vec4 color;

		//	uniform vec3 color_byImGui;

		//	void main()
		//	{
		//		color = vec4(color_byImGui, 1.0);
		//	}
		//)";

		m_Shader.reset(new Overlord::Shader(vertexSrc, FragmentSrcFlat));

		std::string vertexSrc_Blue = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;

				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		//std::string FragmentSrc_Blue = R"(
		//	#version 330 core

		//	in vec3 v_Position;

		//	uniform vec4 u_Color;

		//	layout(location = 0) out vec4 color;

		//	void main()
		//	{
		//		color = u_Color;
		//	}
		//)";

		m_Shader_Blue.reset(new Overlord::Shader(vertexSrc_Blue, FragmentSrcFlat));

		// ==================================================================
	}

	void OnUpdate(Overlord::Timestep ts) override
	{
		//Delta time debug
		//OLD_TRACE("Delta time: {0}s {1}ms", ts.GetSeconds(), ts.GetMilliseconds());

		// Camera rotation input
		if (Overlord::Input::IsKeyPressed(OVLD_KEY_W))
		{
			m_roatation += 100.f * ts;
		}
		else if (Overlord::Input::IsKeyPressed(OVLD_KEY_S))
		{
			m_roatation -= 100.f * ts;
		}

		// Square transform input
		/*if (Overlord::Input::IsKeyPressed(OVLD_KEY_RIGHT))
		{
			m_SquarePosition.x += 1.f * ts;
		}
		else if (Overlord::Input::IsKeyPressed(OVLD_KEY_LEFT))
		{
			m_SquarePosition.x -= 1.f * ts;
		}

		if (Overlord::Input::IsKeyPressed(OVLD_KEY_UP))
		{
			m_SquarePosition.y += 1.f * ts;
		}
		else if (Overlord::Input::IsKeyPressed(OVLD_KEY_DOWN))
		{
			m_SquarePosition.y -= 1.f * ts;
		}*/

		// Render a background color
		Overlord::RenderCommand::SetClearColor({ 0.14f, 0.14f, 0.14f, 1.0f });
		Overlord::RenderCommand::Clear();

		// Rendering
		m_Camera.SetRotation(m_roatation);

		Overlord::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));

		// Colors
		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.f);

		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(x * 0.2f, y * 0.2f, 0.f);
				glm::mat4 transform = glm::translate(glm::mat4(1.f), pos) * scale;

				if (x % 2 == 0)
				{
					m_Shader_Blue->SetFloat4("u_Color", redColor);
				}
				else
				{
					m_Shader_Blue->SetFloat4("u_Color", blueColor);
				}

				Overlord::Renderer::Submit(m_Shader_Blue, m_VA_Square, transform);
			}
		}

		Overlord::Renderer::Submit(m_Shader, m_VertexArray);

		Overlord::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test ImGui Window");
		ImGui::Text("Hello World.");
		ImGui::Button("Click");
		ImGui::End();
	}

	void OnEvent(Overlord::Event& event) override
	{
		
	}

private:
	// Rendering Components
	std::shared_ptr<Overlord::Shader>			m_Shader;
	std::shared_ptr<Overlord::VertexArray>		m_VertexArray;

	std::shared_ptr<Overlord::Shader>			m_Shader_Blue;
	std::shared_ptr<Overlord::VertexArray>		m_VA_Square;

	Overlord::OrthographicCamera m_Camera;

	float m_roatation = 0.f;

	glm::vec3 m_SquarePosition;
};

// ==================================================================

class SandBox : public Overlord::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}

	~SandBox()
	{

	}
};

Overlord::Application* Overlord::CreateApplication()
{
	return new SandBox();
}