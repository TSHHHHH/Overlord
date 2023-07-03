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
		
		void Begin();
		void End();

	private:
		float m_Time = 0.f;
	};
}