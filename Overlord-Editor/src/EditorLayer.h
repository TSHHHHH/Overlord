#pragma once

#include "Overlord.h"

namespace Overlord
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		// Rendering Components
		glm::vec4 m_SquareColor = { 0.f, 0.f, 0.f, 1.f };

		Overlord::Ref<Overlord::Shader>			m_Shader;
		Overlord::Ref<Overlord::VertexArray>	m_VertexArray;

		Overlord::Ref<Overlord::Shader>			m_Shader_Blue;
		Overlord::Ref<Overlord::VertexArray>	m_VA_Square;

		Overlord::Ref<Overlord::Shader>			m_Shader_Tex;
		Overlord::Ref<Overlord::Textur2D>		m_Texture;
		Overlord::Ref<Overlord::Textur2D>		m_Texture_logo;

		Overlord::Ref<Overlord::Framebuffer>	m_Framebuffer;

		// Camera Components
		Overlord::OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition{ 0.f, 0.f, 0.f };
		float m_CameraRotation = 0.f;

		glm::vec3 m_SquarePosition;

		// Editor Components
		glm::vec2 m_ViewportSize;
	};
}
