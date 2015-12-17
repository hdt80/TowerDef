#include "Game.h"
#include "Logger.h"

void Game::start() {
	if (CurrentGameState != Uninitalized) {
		CORE_ERROR("Attempted to start an initalized gamed");
		return; 
	}

	sf::VideoMode currVidMode = sf::VideoMode::getDesktopMode();
	sf::ContextSettings currVidSettings;
	currVidSettings.antialiasingLevel = 8;

	_window.create(sf::VideoMode(900, 600, currVidMode.bitsPerPixel),
		"Tower Defence", sf::Style::Default, currVidSettings);

	loop();
}

void Game::loop() {
	std::chrono::time_point<std::chrono::high_resolution_clock> tStart, tEnd;
	tStart = std::chrono::high_resolution_clock::now();
	long long tDiff;

	while (!toClose()) {
		tEnd = tStart;
		tStart = std::chrono::high_resolution_clock::now();
		tDiff = std::chrono::duration_cast
			<std::chrono::microseconds>(tStart - tEnd).count();

		GameWindow->update(tDiff);
	}
}