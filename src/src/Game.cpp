#include "Game.h"
#include "Logger.h"

#include "GameWindow.h"

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

	CurrentGameState = Playing;
	loop();
}

void Game::loop() {
	std::chrono::time_point<std::chrono::high_resolution_clock> tStart, tEnd;
	tStart = std::chrono::high_resolution_clock::now();
	long long tDiff;

	GameWindow w(Vector2(900, 600));
	w.init();

	followWindow(&w);

	sf::Event e;
	while (!toClose()) {
		tEnd = tStart;
		tStart = std::chrono::high_resolution_clock::now();
		tDiff = std::chrono::duration_cast
			<std::chrono::microseconds>(tStart - tEnd).count();

		// Poll all sf::Events and send them to the Window
		while (_window.pollEvent(e)) {
			// If we're closing (e.g. hit close button) don't let the window
			// handle it, we've got it
			// Likewise, Escape is a hardcoded key that Window's aren't allowed
			// to handle, only the Game can do that. If Shift is pressed at the
			// same time don't go up a level, quit the whole game
			if (e.type == sf::Event::Closed) {
				CurrentGameState = Ending;
			} else if (e.type == sf::Event::KeyPressed &&
				e.key.code == sf::Keyboard::Escape) {

				WindowManager.pop();
				
				if (e.key.shift) {
					CurrentGameState = Ending;
				}
			} else {
				CurrentWindow->handleEvent(e);
			}
		}

		CurrentWindow->update(tDiff);
		CurrentWindow->render(_window);
		//_window.draw(*CurrentWindow);
		Fps.update();
		_window.display();
	}
}

void Game::followWindow(Window* w) {
	WindowManager.push(w);
	CurrentWindow = w;
}

///////////////////////////////////////////////////////////////////////////////
// Static vars
///////////////////////////////////////////////////////////////////////////////
Game::GameState Game::CurrentGameState = Uninitalized;
Window* Game::CurrentWindow            = nullptr;
StateManager Game::WindowManager;
FPS Game::Fps;
sf::RenderWindow Game::_window;