#include <NarcEngine.h>

class Sandbox : public NarcEngine::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

NarcEngine::Application* NarcEngine::CreateApplication()
{
	return new Sandbox();
}