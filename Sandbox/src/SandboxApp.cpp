#include <Overlord.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

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
		Overlord::Ref<Overlord::VertexBuffer> VB(Overlord::VertexBuffer::Create(vertices, sizeof(vertices)));

		Overlord::BufferLayout layout = {
			{ Overlord::ShaderDataType::Float3, "a_Position" },
			{ Overlord::ShaderDataType::Float4, "a_Color" },
		};
		VB->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(VB);

		uint32_t indices[3] = { 0,1,2 };
		Overlord::Ref<Overlord::IndexBuffer> IB(Overlord::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(IB);

		// Try to render a square
		m_VA_Square.reset(Overlord::VertexArray::Create());

		float vertices_square[4 * 5] = {
			// Vertex Pos         // Texture
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Overlord::Ref<Overlord::VertexBuffer> VB_Square(Overlord::VertexBuffer::Create(vertices_square, sizeof(vertices_square)));

		VB_Square->SetLayout({
			{ Overlord::ShaderDataType::Float3, "a_Position" },
			{ Overlord::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_VA_Square->AddVertexBuffer(VB_Square);

		uint32_t indices_square[6] = { 0, 1, 2, 2, 3, 0 };
		Overlord::Ref<Overlord::IndexBuffer> m_IB_Square(Overlord::IndexBuffer::Create(indices_square, sizeof(indices_square) / sizeof(uint32_t)));
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

				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_Shader.reset(Overlord::Shader::Create(vertexSrc, FragmentSrcFlat));

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

		m_Shader_Blue.reset(Overlord::Shader::Create(vertexSrc_Blue, FragmentSrcFlat));

		std::string vertexSrc_TextureShader = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;

				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string FragmentSrc_TextureShader = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_Shader_Tex.reset(Overlord::Shader::Create(vertexSrc_TextureShader, FragmentSrc_TextureShader));

		m_Texture = Overlord::Textur2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Overlord::OpenGLShader>(m_Shader_Tex)->Use();
		std::dynamic_pointer_cast<Overlord::OpenGLShader>(m_Shader_Tex)->SetInt("u_Texture", 0);

		// ==================================================================
	}

	void OnUpdate(Overlord::Timestep ts) override
	{
		//Delta time debug
		//OLD_TRACE("Delta time: {0}s {1}ms", ts.GetSeconds(), ts.GetMilliseconds());

		// Camera position + rotation input
		if (Overlord::Input::IsKeyPressed(OVLD_KEY_A))
		{
			m_CameraPosition.x -= 1.f * ts;
		}
		else if (Overlord::Input::IsKeyPressed(OVLD_KEY_D))
		{
			m_CameraPosition.x += 1.f * ts;
		}

		if (Overlord::Input::IsKeyPressed(OVLD_KEY_W))
		{
			m_CameraPosition.y += 1.f * ts;
		}
		else if (Overlord::Input::IsKeyPressed(OVLD_KEY_S))
		{
			m_CameraPosition.y -= 1.f * ts;
		}

		if (Overlord::Input::IsKeyPressed(OVLD_KEY_E))
		{
			m_CameraRotation += 100.f * ts;
		}
		else if (Overlord::Input::IsKeyPressed(OVLD_KEY_Q))
		{
			m_CameraRotation -= 100.f * ts;
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
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Overlord::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));

		// Colors
		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.f);

		//std::dynamic_pointer_cast<Overlord::OpenGLShader>(m_Shader)->Use();
		//std::dynamic_pointer_cast<Overlord::OpenGLShader>(m_Shader)->SetFloat4("u_Color", redColor);

		std::dynamic_pointer_cast<Overlord::OpenGLShader>(m_Shader_Blue)->Use();
		std::dynamic_pointer_cast<Overlord::OpenGLShader>(m_Shader_Blue)->SetFloat4("u_Color", m_SquareColor);



		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(x * 0.15f, y * 0.15f, 0.f);
				glm::mat4 transform = glm::translate(glm::mat4(1.f), pos) * scale;

				//if (x % 2 == 0)
				//{
				//	std::dynamic_pointer_cast<OpenGLShader>(m_Shader_Blue)->SetFloat4("u_Color", redColor);
				//}
				//else
				//{
				//	std::dynamic_pointer_cast<OpenGLShader>(m_Shader_Blue)->SetFloat4("u_Color", blueColor);
				//}

				Overlord::Renderer::Submit(m_Shader_Blue, m_VA_Square, transform);
			}
		}

		m_Texture->Bind();
		Overlord::Renderer::Submit(m_Shader_Tex, m_VA_Square, glm::scale(glm::mat4(1.f), glm::vec3(1.5f)));

		//Triangle
		//Overlord::Renderer::Submit(m_Shader, m_VertexArray);

		Overlord::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test ImGui Window");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	void OnEvent(Overlord::Event& event) override
	{
		
	}

private:
	// Rendering Components
	glm::vec4 m_SquareColor = { 0.f, 0.f, 0.f, 1.f };

	Overlord::Ref<Overlord::Shader>			m_Shader;
	Overlord::Ref<Overlord::VertexArray>	m_VertexArray;

	Overlord::Ref<Overlord::Shader>			m_Shader_Blue;
	Overlord::Ref<Overlord::VertexArray>	m_VA_Square;

	Overlord::Ref<Overlord::Shader>			m_Shader_Tex;
	Overlord::Ref<Overlord::Textur2D>		m_Texture;

	Overlord::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition{ 0.f, 0.f, 0.f };
	float m_CameraRotation = 0.f;

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