#pragma once

#include "core.h"
#include "Events/Event.h"

namespace Overlord
{
	class OLD_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be define in client
	Application* CreateApplication();
}
