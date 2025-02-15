#include <NarcEngine.h>

class Sandbox : public narc_engine::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

narc_engine::Application* narc_engine::createApplication()
{
	return new Sandbox();
}