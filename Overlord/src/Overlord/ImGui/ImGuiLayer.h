#pragma once

#include "Overlord/Layer.h"

#include "Overlord/Events/ApplicationEvent.h"
#include "Overlord/Events/KeyEvent.h"
#include "Overlord/Events/MouseEvent.h"

namespace Overlord
{
	class OLD_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		// Inout events
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);
		bool OnMousePressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);

		float m_Time = 0.f;
	};
}