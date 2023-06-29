#include <Overlord.h>

class ExampleLayer : public Overlord::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer")
	{

	}

	void OnUpdate() override
	{
		if (Overlord::Input::IsKeyPressed(OVLD_KEY_TAB))
		{
			OLD_TRACE("Tab key is pressed");
		}
	}

	void OnEvent(Overlord::Event& event) override
	{
		// OLD_TRACE("{0}", event);
	}
};

class SandBox : public Overlord::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Overlord::ImGuiLayer());
	}

	~SandBox()
	{

	}
};

Overlord::Application* Overlord::CreateApplication()
{
	return new SandBox();
}