#ifndef _GAME_H
#define _GAME_H

#include <SFML/Graphics.hpp>
#include "Window.h"

class Game {
public:
	static void start();

	static bool toClose() { return CurrentGameState == Game::Ending; }
	static void loop();

	enum GameState {Uninitalized, Paused, InMenu, Playing, Ending};

	static GameState CurrentGameState;
	static Window* GameWindow;
private:
	static sf::RenderWindow _window; 
};

#endif