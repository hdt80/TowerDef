// =================================
// Includes
// =================================
#define DO_DEBUG

#include <stdlib.h>
#include <random>
#include <time.h>

#include "Window.h"
#include "Logger.h"

// =================================
// Main
// =================================
int main(int argc, char* argv[]) {
	srand(time(0));
	Logger::instance().setLogFile("log.html", false, true);

	// The window must be created on the main thread due to OS limitations
   	Window w("Tower", 900, 600);
   	w.start();
	return 0;
}
