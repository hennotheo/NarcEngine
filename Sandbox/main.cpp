#include <iostream>

namespace NarcEngine
{
	__declspec(dllimport) void Test();
}

int main()
{
	NarcEngine::Test();
	std::cin.get();

	return EXIT_SUCCESS;
}