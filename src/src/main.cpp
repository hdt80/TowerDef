// =================================
// Includes
// =================================
#define DO_DEBUG

#include <stdlib.h>
#include <random>

#include "Logger.h"

#include "Game.h"

// =================================
// Main
// =================================
int main(int argc, char* argv[]) {
	srand(time(0));
	Logger::instance().setLogFile("log.html", false, true);

	// The window must be created on the main thread due to OS limitations
   	Game::start();
	return 0;
}
