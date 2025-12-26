#include "OmegaEngine/Engine.h"
#include <iostream>
#include <stdexcept>

int main()
{
	std::cout << "Hello world!" << std::endl;
	Omega::Engine engine = Omega::Engine();
	try {
		engine.Init();
		engine.Shutdown();
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}