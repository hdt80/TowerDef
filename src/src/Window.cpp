#include "Window.h"

#include "Logger.h"
#include "Convert.h"

#include "sfLine.h"
#include "Vector2.h"
#include "Enemy.h"
#include "ParticleEmitter.h"
#include "Perk.h"
#include "SkillTree.h"

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
	_pauseColor(128, 128, 128, 128), _enemyColor(sf::Color::Red),
	_towerColor(sf::Color::Green), _tracerColor(255, 0, 0, 128),
	_towerRangeColor(127, 255, 127, 127), _projectileColor(120, 120, 120),
	_selected(nullptr), _showTree(false)  {

	sf::VideoMode currVidMode = sf::VideoMode::getDesktopMode();
	sf::ContextSettings currVidSettings;
	currVidSettings.antialiasingLevel = 8;

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

	ParticleEmit::window = this; // Tell the ParticleEmitter what window to use
	_map.setSize(_width, _height);
	SkillTrees::createTrees(Vector2(_width, _height));
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
		// Timing variables
		tEnd = tStart;
		tStart = std::chrono::high_resolution_clock::now();
		tDiff = std::chrono::duration_cast
			<std::chrono::microseconds>(tStart - tEnd).count();

		// Real loop
		pollEvents();
		if (!_paused) {
			for (unsigned int i = 0; i < emitters.size(); ++i) {
				emitters[i]->update(tDiff);
			}
			_map.update(tDiff);
		}
		render();

		_fps.update();
		_window.setTitle(convert::toString(_fps.getFPS())
			+ " : " + convert::toString(_map.getHealth())
			);
		// End real loop

		// If 16666ms haven't passed yet sleep for the time left
		tEnd = std::chrono::high_resolution_clock::now();
		if(tEnd - (std::chrono::microseconds(REFRESH_RATE)) < tStart) {
			//std::this_thread::sleep_for(
			//	std::chrono::microseconds(REFRESH_RATE) - (tEnd - tStart));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Main render method
///////////////////////////////////////////////////////////////////////////////
void Window::render() {
	if (!_paused) {
		_window.clear(sf::Color::Black); // Remove anything thats on the window

		updateEmitters();

		if (_showTree && _selected && _selected->getTree() != nullptr) {
			CORE_INFO("%x", _selected->getTree());	
			_window.draw(*_selected->getTree());
		} else {
			renderMap();
			renderSelected();
			for (unsigned int i = 0; i < _map.objects.size(); ++i) {
				_window.draw(*_map.objects[i]);
			}
			for (unsigned int i = 0; i < emitters.size(); ++i) {
				_window.draw(*emitters[i]);
			}
		}

		_window.display(); // After we're done the drawing end the current frame
	} else {
		// Don't continually draw the pause screen as it will cover the screen 
		// and make it black, so just draw it once
		if (!_pausedDrawn) {
			sf::RectangleShape box(sf::Vector2f(_width, _height));
			box.setPosition(0, 0);
			box.setFillColor(_pauseColor);
			_window.draw(box);
			_window.display();
			_pausedDrawn = true;
		}
	}
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
}

void Window::renderSelected() {
	// Don't draw anything if nothing is selected
	if (_selected == nullptr) {
		return;
	}

	// Draw range of the object
	sf::CircleShape r(_selected->getRange());
	r.setPosition(_selected->getX() - _selected->getRange(),
		_selected->getY() - _selected->getRange());

	r.setFillColor(_towerRangeColor);
	_window.draw(r);

	// Tower is shooting at something? Draw on top of range
	if (_selected->getTarget() != nullptr) {
		sfLine l(sf::Vector2f(_selected->getX(), _selected->getY()),
			sf::Vector2f(_selected->getTarget()->getX(),
			_selected->getTarget()->getY()), 1, _tracerColor);

		_window.draw(l);
	}

	// Drawing perks

	// Box we'll be drawing everything from
	sf::RectangleShape box(sf::Vector2f(PERK_BOX_WIDTH, _height));

	// Draw background box
	box.setFillColor(sf::Color(112, 112, 112));
	box.setOutlineColor(sf::Color(200, 200, 200));
	box.setOutlineThickness(2); // Defaults to 0, or no outline
	box.setPosition(_width - PERK_BOX_WIDTH, 0);
	_window.draw(box);

	// Draw each perk, with stack count and progress bar
	for (unsigned int i = 0; i < _selected->perkCount(); ++i) {
		// Max of 8 perks
		box.setSize(sf::Vector2f(PERK_BOX_WIDTH, _height / 8));
		box.setFillColor(sf::Color(64, 64, 64));
		box.setPosition(_width - PERK_BOX_WIDTH, (_height / 8) * i);

		// Draw we'll draw the name to
		sf::Text text(_selected->getPerk(i)->getTitle(), _font);
		text.setPosition(box.getPosition());
		text.setColor(sf::Color(255, 255, 255));
		_window.draw(box);
		_window.draw(text);

		// Draw red background for duration bar
		box.setFillColor(sf::Color::Red);
		// Bar fills up half of the box, so 1/16 is half a box
		box.setSize(sf::Vector2f(PERK_BOX_WIDTH, _height / 16));
		box.setPosition(_width - PERK_BOX_WIDTH,
			(_height / 16) + ((_height / 8) * i));
		_window.draw(box);

		// Draw green background for duration bar
		float percent = _selected->getPerk(i)->getDuration() /
			_selected->getPerk(i)->getMaxDuration();
		box.setFillColor(sf::Color::Green);
		box.setSize(sf::Vector2f(PERK_BOX_WIDTH * percent, _height / 16));
		_window.draw(box);
	}
}

// Remove any unneeded emitters
void Window::updateEmitters() {
	for (unsigned int i = 0; i < emitters.size(); ++i) {
		if (emitters[i]->done() == true) {
			delete emitters[i];
			emitters.erase(emitters.begin() + i);
		}
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
		_pausedDrawn = false;
	} else if (e.key.code == sf::Keyboard::T) {
		if (_selected) {
			_showTree = !_showTree;
		}
	}
}

void Window::mouseEvent(sf::Event e) {
	int x = e.mouseButton.x;
	int y = e.mouseButton.y;
	if (_showTree) {
		if (_selected && _selected->getTree()) {
			SkillNode* node = _selected->getTree()->getNode(x, y);
			node->print();
		}
	}
	if (e.mouseButton.button == sf::Mouse::Left) {
		if (_map.towerAt(x, y) != nullptr) {
			_selected = _map.towerAt(x, y);
		} else {
			// If we've selected something we just want to deselected it
			if (_selected != nullptr) {
				_selected = nullptr;
			} else {
				_map.spawnTower(x, y);
			}
		}
	} else if (e.mouseButton.button == sf::Mouse::Middle) {
		_map.getPath()->addPoint(x, y);
	}

}

void Window::resizeEvent(sf::Event e) {
	CORE_INFO("Resize: %i, %i => %i, %i", _width, _height,
		e.size.width, e.size.height);
	_width  = e.size.width;
	_height = e.size.height;
	//updateAreaSizes();
}
