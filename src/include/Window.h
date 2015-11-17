#ifndef _RENDER_WINDOW_H
#define _RENDER_WINDOW_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

#include "Map.h" // Map, Tower*
#include "FPS.h" // FPS
#include "Vector2.h"
#include "Particle.h"
#include "Common.h"

// #define PATH_WIDTH  25 // Width in pixels
// #define ENEMY_WIDTH 10 // Radius in pixels
// #define TOWER_WIDTH 10
// #define PROJECTILE_WIDTH 3
// #define PERK_BOX_WIDTH 150

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
	void setClose(bool b) { _close = b; }

	std::vector<ParticleEmitter*> emitters;
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

	// Predefined colors
	sf::Color _pauseColor;
	sf::Color _enemyColor;
	sf::Color _towerColor;
	sf::Color _tracerColor;
	sf::Color _towerRangeColor;
	sf::Color _projectileColor;

	Tower* _selected; // Tower we've clicked on and want to see it's stats

	// We split the code up so it's easier to fix when stuff breaks
	void renderMap();
	void renderSelected();
	void renderEnemies();
	void renderTowers();
	void renderProjectiles();

	// Update all emitters and remove any that are done
	void updateEmitters();
};

#endif