#pragma once

#include "Core.h"

namespace NarcEngine
{
	class NARC_ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}