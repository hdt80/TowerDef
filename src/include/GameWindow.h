#ifndef _GAME_WINDOW_H
#define _GAME_WINDOW_H

#include "Window.h"
#include "Map.h"

class GameWindow : public Window {
public:
	GameWindow(Vector2 size = Vector2(0.0f, 0.0f));
	~GameWindow()

	void init();
	void restart();
	void pause();
	void resume();
	void close();

	void update(int diff);
	void pollEvents();

	void keyEvent(sf::Event& e);
	void mouseEvent(sf::Event& e);
	void resizeEvent(sf::Event& e);

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	Map _map;
};

#endif