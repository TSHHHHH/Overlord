#include <Overlord.h>

class ExampleLayer : public Overlord::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		OLD_INFO("Example Layer Update");
	}

	void OnEvent(Overlord::Event& event) override
	{
		OLD_TRACE("{0}", event);
	}
};

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