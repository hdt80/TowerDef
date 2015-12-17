#ifndef _WINDOW_H
#define _WINDOW_H

#include <SFML/Graphics.hpp>

#include "Vector2.h"

class Window : sf::Drawable, sf::Transformable {
public:
	Window(Vector2 size = Vector2(0.0f, 0.0f));
	virtual ~Window() {}

	enum WindowState {Running, Paused, Uninitalized};

	// Initalize the window 
	virtual void init();
	// Reinit the Window
	virtual void restart();
	// Pause the window, stop updating
	virtual void pause();
	// Resume updating
	virtual void resume();
	// Mark the Window for closing
	virtual void close();

	// Update the Window. 
	// diff is the amount of microseconds that have passed since the last call
	virtual void update(int diff);
	virtual void pollEvents();

	// Event methods
	virtual void keyEvent(sf::Event& e);
	virtual void mouseEvent(sf::Event& e);
	virtual void resizeEvent(sf::Event& e);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	Vector2 _size; // width and height of the Window
	WindowState _currState;
};


#endif