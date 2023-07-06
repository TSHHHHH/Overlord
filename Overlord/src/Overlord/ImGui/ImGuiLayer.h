#pragma once

#include "Overlord/Layer.h"

#include "Overlord/Events/ApplicationEvent.h"
#include "Overlord/Events/KeyEvent.h"
#include "Overlord/Events/MouseEvent.h"

// Temp
//#include <glm/glm.hpp>

namespace Overlord
{
	// Temp
	//extern glm::vec3 position;
	//extern glm::vec3 color;

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