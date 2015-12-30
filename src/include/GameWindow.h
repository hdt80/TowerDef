#ifndef _GAME_WINDOW_H
#define _GAME_WINDOW_H

#include "Window.h"
#include "Map.h"
#include "Particle.h"
#include "Vector2.h"

class GameWindow : public Window {
public:
	GameWindow(Vector2 size = Vector2(0.0f, 0.0f));
	~GameWindow();

	// Initalize the window 
	virtual void init();
	// // Reinit the Window
	// virtual void reset();
	// // Pause the window, stop updating
	// virtual void pause();
	// // Resume updating
	// virtual void resume();
	// // Mark the Window for closing
	// virtual void close();

	// Update the Window. 
	// diff is the amount of microseconds that have passed since the last call
	virtual void update(int diff);

	// Event methods
	virtual void handleEvent(sf::Event& e);
	virtual void keyEvent(sf::Event& e);
	virtual void mouseEvent(sf::Event& e);
	//virtual void resizeEvent(sf::Event& e);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	virtual void render(sf::RenderWindow& window);

	std::vector<ParticleEmitter*> emitters;

protected:
	// Rendering submethods
	void renderMap(sf::RenderWindow&);
	void renderSelected(sf::RenderWindow&);

	sf::Font _font;

	Map _map;

	// Tower we've selected with our mouse and want to see it's Perks
	Tower* _selected;

};

#endif