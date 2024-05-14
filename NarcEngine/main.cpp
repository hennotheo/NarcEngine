//#include "Engine.h"
#include <iostream>

namespace NarcEngine
{
	__declspec(dllexport) void Test()
	{
		std::cout << "TEST" << std::endl;
	}
}

//int main()
//{
//	NarcEngine::Engine engine;
//
//	try
//	{
//		engine.Run();
//	}
//	catch (const std::exception& e)
//	{
//		std::cerr << e.what() << std::endl;
//		std::cin.get();
//		return EXIT_FAILURE;
//	}
//
//	return EXIT_SUCCESS;
//}