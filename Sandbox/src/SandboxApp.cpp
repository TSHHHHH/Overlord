#include <Overlord.h>

class SandBox : public Overlord::Application
{
public:
	SandBox()
	{

	}

	~SandBox()
	{

	}
};

Overlord::Application* Overlord::CreateApplication()
{
	return new SandBox();
}