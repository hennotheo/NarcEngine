#pragma once

namespace narc_engine
{
	class NARC_ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	Application* createApplication();
}