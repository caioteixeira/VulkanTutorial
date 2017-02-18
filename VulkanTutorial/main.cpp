#include "Renderer.h"
#include <iostream>

int main() {
	Renderer renderer;

	try {
		renderer.runRenderer();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
