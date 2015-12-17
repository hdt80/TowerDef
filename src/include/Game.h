#ifndef _GAME_H
#define _GAME_H

#include <SFML/Graphics.hpp>
#include <chrono>

#include "Window.h"
#include "Vector2.h"

#define REFRESH_RATE 16666 // 60 FPS - 16.666ms between each frame

class Game {
public:
	static void start();
	static void loop();

	static bool toClose() { return CurrentGameState == Game::Ending; }

	static void followWindow(Window* w);

	enum GameState {Uninitalized, Paused, InMenu, Playing, Ending};

	static GameState CurrentGameState;
	static Window* GameWindow;

private:
	static sf::RenderWindow _window;

	Vector2 _size;
};

#endif