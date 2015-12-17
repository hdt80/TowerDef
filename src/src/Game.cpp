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
}

void Game::loop() {
	GameWindow->update();
}