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
	virtual void init() = 0;
	// Reinit the Window
	virtual void restart() = 0;
	// Pause the window, stop updating
	virtual void pause() = 0;
	// Resume updating
	virtual void resume() = 0;
	// Mark the Window for closing
	virtual void close() = 0;

	// Update the Window. 
	// diff is the amount of microseconds that have passed since the last call
	virtual void update(int diff) = 0;
	virtual void pollEvents() = 0;

	// Event methods
	virtual void keyEvent(sf::Event& e) = 0;
	virtual void mouseEvent(sf::Event& e) = 0;
	virtual void resizeEvent(sf::Event& e) = 0;

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

private:
	Vector2 _size; // width and height of the Window
	WindowState _currState;
};


#endif