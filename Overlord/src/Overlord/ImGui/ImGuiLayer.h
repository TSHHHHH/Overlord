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

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;

		virtual void OnEvent(Event& e) override;
		
		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = false;
		float m_Time = 0.f;
	};
}