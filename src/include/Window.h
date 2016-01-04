#ifndef _WINDOW_H
#define _WINDOW_H

#include <SFML/Graphics.hpp>

#include "Vector2.h"

// Window
// A Window is a state the Game is in that determines what game flow
// should be followed. The Game holds what Window we're using and controls
// movement between Windows. 
class Window : public sf::Drawable, public sf::Transformable {
public:
	Window(const std::string& name, Vector2 size = Vector2(0.0f, 0.0f));
	virtual ~Window();

	enum WindowState {Running, Paused, Uninitalized};

	// Initalize the window.
	// Called when the Window is first pushed onto the state stack and another
	// Window of the same type isn't on the state stack
	// init will be called right before it is made the current state
	// in StateManager
	virtual void init();
	// Reinit the Window
	// Reinitalize the Window and reset it to default values
	virtual void reset();
	// Pause the window, stop updating
	// Called when we change our current Window to another one
	virtual void pause();
	// Resume updating
	// This Window has been initalized but paused
	virtual void resume();
	// Close the Window, it is no longer being used
	// Clean up everything and deallocate all associated objects
	virtual void close();

	// Change the Window's state
	virtual void setState(WindowState state);
	const WindowState getState() { return _currState; }
	const std::string getStateString();
	const std::string getStateString(WindowState state);

	void setSize(Vector2 size) { _size = size; }
	int getWidth() { return _size.X; }
	int getHeight() { return _size.Y; }

	// Name getter and setter
	const std::string& getName() { return _name; }
	void setName(const std::string& name) { _name = name; }

	// Update the Window
	// diff is the amount of microseconds that have passed since the last call
	virtual void update(int diff);

	// Event methods
	virtual void handleEvent(sf::Event& e);
	virtual void keyEvent(sf::Event& e);
	virtual void mouseEvent(sf::Event& e);
	virtual void resizeEvent(sf::Event& e);

	// Drawing
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	// 
	virtual void render(sf::RenderWindow&);

protected:
	Window();

	std::string _name;

	Vector2 _size; // width and height of the Window
	WindowState _currState;
};


#endif