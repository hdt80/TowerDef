#ifndef _RENDER_WINDOW_H
#define _RENDER_WINDOW_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

#include "Map.h"
#include "FPS.h"
#include "Vector2.h"

#define PATH_WIDTH  25 // Width in pixels
#define ENEMY_WIDTH 10 // Radius
#define TOWER_WIDTH 10

class Window {
public:
	Window(std::string name, int width, int height, bool fullscreen = false);
	~Window();

	// Control methods
	void start();
	void loop();

	// Main loop methods
	void render();
	void pollEvents();

	// Event methods
	void keyEvent(sf::Event e);
	void mouseEvent(sf::Event e);
	void resizeEvent(sf::Event e);

	bool shouldClose() { return _close; }
	void setClose(bool b) {_close = b; }
private:
	bool _close; // If the Window is queued to close
	bool _paused;
	bool _pausedDrawn; // If the paused box is drawn
	int  _width;
	int  _height;

	Map _map;
	FPS _fps;

	sf::RenderWindow _window;
	sf::Font _font;
	sf::Color _pauseColor;

	// We split the code up so it's easier to fix when stuff breaks
	void renderMap();
};

#endif