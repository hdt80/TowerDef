#include "Window.h"

#include "Logger.h"
#include "Convert.h"

#include "sfLine.h"
#include "Vector2.h"
#include "Enemy.h"

#include <chrono>
#include <thread>
#include <cmath>

#include <SFML/System.hpp>

#define REFRESH_RATE 16666 // 60 FPS - 16.6667ms between each frame

///////////////////////////////////////////////////////////////////////////////
// Constuctor
///////////////////////////////////////////////////////////////////////////////
Window::Window(std::string name, int w, int h, bool fullscreen) :
	_close(false), _paused(false), _width(w), _height(h),
	_pauseColor(128, 128, 128, 128) {

	sf::VideoMode currVidMode = sf::VideoMode::getDesktopMode();
	sf::ContextSettings currVidSettings;
	//currVidSettings.antialiasingLevel = 8;

	if (fullscreen) {
		_window.create(sf::VideoMode(_width, _height, currVidMode.bitsPerPixel),
			name, sf::Style::Fullscreen, currVidSettings);
	} else {
		_window.create(sf::VideoMode(_width, _height, currVidMode.bitsPerPixel),
			name, sf::Style::Default, currVidSettings);
	}
	//_window.setFramerateLimit(60);

	if (!_font.loadFromFile("res\\Pixel.ttf")) {
		CORE_ERROR("Failed to load \'%s\\res\\Pixel.ttf\'", 
			convert::getWorkingDir().c_str());
	}

	CORE_INFO("Created window: Width: %i, Height: %i, Bits per Pixel: %i",
		_width, _height, currVidMode.bitsPerPixel);
}

Window::~Window() {

}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Window::start() {
	render();
	loop();
}

void Window::loop() {
	std::chrono::time_point<std::chrono::high_resolution_clock> tStart, tEnd;
	tStart = std::chrono::high_resolution_clock::now();
	long long tDiff;

	_map.spawnWave();

	while (!shouldClose()) {
		tEnd = tStart;
		tStart = std::chrono::high_resolution_clock::now();
		tDiff = std::chrono::duration_cast
			<std::chrono::microseconds>(tStart - tEnd).count();

		// Real loop
		pollEvents();
		if (!_paused) {
			_map.update(tDiff);
		}
		render();

		_fps.update();
		_window.setTitle(convert::toString(_fps.getFPS()));
		// End real loop

		// If 16666ms haven't passed yet sleep for the time left
		tEnd = std::chrono::high_resolution_clock::now();
		if(tEnd - (std::chrono::microseconds(REFRESH_RATE)) < tStart) {
			std::this_thread::sleep_for(
				std::chrono::microseconds(REFRESH_RATE) - (tEnd - tStart));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Main render method
///////////////////////////////////////////////////////////////////////////////
void Window::render() {
	if (!_paused) {
		_window.clear(sf::Color::Black); // Remove anything that's on the window

		renderMap();
	} else {
		sf::RectangleShape box(sf::Vector2f(_width, _height));
		box.setPosition(0, 0);
		box.setFillColor(_pauseColor);
		_window.draw(box);
	}

	_window.display(); // After we're done the drawing end the current frame
}

///////////////////////////////////////////////////////////////////////////////
// Render sub methods
///////////////////////////////////////////////////////////////////////////////
void Window::renderMap() {
	// Drawing the path enemies will take
	Path* path = _map.getPath();

	// Start the path at the first point
	sf::Vector2f prev(path->getPoint(0)->X, path->getPoint(0)->Y);
	sf::Vector2f curr(0, 0); // Current point we're drawing the line to

	// Start at the second point, we've already gotten to first point
	for (unsigned int i = 1; i < path->size(); ++i) {
		curr.x = path->getPoint(i)->X;
		curr.y = path->getPoint(i)->Y;

		sfLine s(prev, curr, PATH_WIDTH);

		// Create a circle at the end of the line for curved edges
		// Radius is the thickness / 2
		sf::CircleShape c(PATH_WIDTH / 2);
		// Position is based on the top left point, so center it based on
		// the radius
		c.setPosition(sf::Vector2f(curr.x - PATH_WIDTH / 2, 
								   curr.y - PATH_WIDTH / 2));
		prev = curr;

		_window.draw(s);
		_window.draw(c);
	}

	// Drawing all the enemies on the map
	Enemy* e = nullptr;
	sf::CircleShape s(ENEMY_WIDTH);
	s.setFillColor(sf::Color::Red);
	for (unsigned int i = 0; i < _map.enemies.size(); ++i) {
		e = _map.enemies[i];
		s.setPosition(e->getX() - ENEMY_WIDTH, e->getY() - ENEMY_WIDTH);

		_window.draw(s);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Methods relating to events
///////////////////////////////////////////////////////////////////////////////

// Poll all queued events and act on them
void Window::pollEvents() {
	sf::Event e; // Event we'll send the polling to

	while (_window.pollEvent(e)) {
		if (e.type == sf::Event::Closed) {
			setClose(true);
		} else if (e.type == sf::Event::KeyPressed) {
			keyEvent(e);
		} else if (e.type == sf::Event::MouseButtonReleased) {
			mouseEvent(e);
		} else if (e.type == sf::Event::Resized) {
			resizeEvent(e);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Event handlers
///////////////////////////////////////////////////////////////////////////////
void Window::keyEvent(sf::Event e) {
	if (e.key.code == sf::Keyboard::Escape) {
		setClose(true);
	} else if (e.key.code == sf::Keyboard::N) {
		_map.spawnWave();
	} else if (e.key.code == sf::Keyboard::P) {
		_paused = !_paused;
	}
}

void Window::mouseEvent(sf::Event e) {
	CORE_INFO("CLICK: (%i, %i)", e.mouseButton.x, e.mouseButton.y);
}

void Window::resizeEvent(sf::Event e) {
	CORE_INFO("Resize: %i, %i => %i, %i", _width, _height,
		e.size.width, e.size.height);
	_width  = e.size.width;
	_height = e.size.height;
	//updateAreaSizes();
}