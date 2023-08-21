#include "Overlord.h"
#include "Overlord/EntryPoint.h"

#include "EditorLayer.h"

namespace Overlord
{
	class OverlordEditor : public Application
	{
	public:
		OverlordEditor()
			: Application("Overlord Editor")
		{
			PushLayer(new EditorLayer());
		}

		~OverlordEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new OverlordEditor();
	}
}
