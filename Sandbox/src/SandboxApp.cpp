#include <Overlord.h>

#include "imgui/imgui.h"

class ExampleLayer : public Overlord::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer")
	{

	}

	void OnUpdate() override
	{
		//if (Overlord::Input::IsKeyPressed(OVLD_KEY_TAB))
		//{
		//	OLD_TRACE("Tab key is pressed");
		//}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World.");
		ImGui::Button("Click");
		ImGui::End();
	}

	void OnEvent(Overlord::Event& event) override
	{
		// Debug
		OLD_TRACE("{0}", event.GetName());
	}
};

class SandBox : public Overlord::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
		// PushOverlay(new Overlord::ImGuiLayer());
	}

	~SandBox()
	{

	}
};

Overlord::Application* Overlord::CreateApplication()
{
	return new SandBox();
}