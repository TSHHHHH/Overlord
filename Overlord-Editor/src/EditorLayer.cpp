#include "EditorLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

namespace Overlord
{
	EditorLayer::EditorLayer()
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
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f
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
		m_Texture_logo = Overlord::Textur2D::Create("assets/textures/Logo.png");

		std::dynamic_pointer_cast<Overlord::OpenGLShader>(m_Shader_Tex)->Use();
		std::dynamic_pointer_cast<Overlord::OpenGLShader>(m_Shader_Tex)->SetInt("u_Texture", 0);

		// ==================================================================

		Overlord::FramebufferSpec fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Overlord::Framebuffer::Create(fbSpec);
	}

	void EditorLayer::OnAttach()
	{

	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep ts)
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

		m_Framebuffer->Bind();

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
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.f);
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
		m_Texture_logo->Bind();
		Overlord::Renderer::Submit(m_Shader_Tex, m_VA_Square, glm::scale(glm::mat4(1.f), glm::vec3(1.5f)));

		//Triangle
		//Overlord::Renderer::Submit(m_Shader, m_VertexArray);

		Overlord::Renderer::EndScene();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		// Docking space
		// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
		// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
		// In this specific demo, we are not using DockSpaceOverViewport() because:
		// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
		// - we allow the host window to have padding (when opt_padding == true)
		// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
		// TL;DR; this demo is more complicated than what you would normally use.
		// If we removed all the options we are showcasing, this demo would become:
		//     void ShowExampleAppDockSpace()
		//     {
		//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		//     }

		static bool p_open = true;

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &p_open, window_flags);

		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			// ShowDockingDisabledMessage();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Overlord::Application::Get().ShutDownApplication();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// Color picker
		{
			ImGui::Begin("Test ImGui Window");

			ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

			ImGui::End();
		}

		// Viewport
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");

			glm::vec2 viewportPanelSize = *((glm::vec2*)&ImGui::GetContentRegionAvail());

			if (m_ViewportSize != viewportPanelSize)
			{
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

				// Resize camera HERE
			}

			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			ImGui::End();
			ImGui::PopStyleVar();
		}

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{

	}
}
