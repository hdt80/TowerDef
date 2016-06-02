#include "GameWindow.h"
#include "Logger.h"
#include "Game.h"
#include "sfLine.h"
#include "Common.h"
#include "SkillTree.h"
#include "ParticleEmitter.h"
#include "Convert.h"
#include "SkillTreeWindow.h"
#include "GuiComponent.h"
#include "components/SelectedComponent.h"

///////////////////////////////////////////////////////////////////////////////
// Creation Methods
///////////////////////////////////////////////////////////////////////////////
GameWindow::GameWindow(Vector2 size) {
	_size = size;
	_name = "Game Window";
	_map.setSize(size.X, size.Y);
	_selected = nullptr;

	SelectedComponent* selcComp = new SelectedComponent(this);

	_components.push_back(selcComp);
	CORE_INFO("game window: %x", &_selected);
}

GameWindow::~GameWindow() {

}

///////////////////////////////////////////////////////////////////////////////
// State Methods
///////////////////////////////////////////////////////////////////////////////
void GameWindow::init() {
	Window::init();
	SkillTrees::createTrees(_size);
	ParticleEmit::window = this;
	
	if (!_font.loadFromFile("res\\Pixel.ttf")) {
		CORE_ERROR("Failed to load \'%s\\res\\Pixel.ttf\'",
			convert::getWorkingDir().c_str());
	}
}

///////////////////////////////////////////////////////////////////////////////
// Updating Methods
///////////////////////////////////////////////////////////////////////////////
void GameWindow::update(int diff) {
	Window::update(diff);
	for (unsigned int i = 0; i < emitters.size(); ++i) {
		emitters[i]->update(diff);
	}
	_map.update(diff);
}

///////////////////////////////////////////////////////////////////////////////
// SFML Method Wrapper
///////////////////////////////////////////////////////////////////////////////
void GameWindow::handleEvent(sf::Event& e) {
	Window::handleEvent(e);
}

void GameWindow::keyEvent(sf::Event& e) {
	if (e.key.code == sf::Keyboard::Escape) {
		CORE_INFO("ESC");
	} else if (e.key.code == sf::Keyboard::N) {
		_map.spawnWave();
	} else if (e.key.code == sf::Keyboard::T) {
        if (_selected) {
            Game::followWindow(new SkillTreeWindow(_selected->getTree(), _size));
        }
	} else if (e.key.code == sf::Keyboard::Pause) {
		Game::pause();
	} else if (e.key.code == sf::Keyboard::S) {
        if (_selected) {
            CORE_INFO("\nStats: ");
            _selected->getStats().print();
            CORE_INFO("\nbaseStats: ");
            _selected->getBaseStats().print();
			CORE_INFO("\nmodStats: ");
			_selected->getStatMod().print();
        }
    }
}

void GameWindow::mouseEvent(sf::Event& e) {
	int x = e.mouseButton.x;
	int y = e.mouseButton.y;
	if (e.mouseButton.button == sf::Mouse::Left) {
		// If there is a Tower where we clicked, selected that
		if (_map.towerAt(x, y) != nullptr) {
			_selected = _map.towerAt(x, y);
		} else {
			// If there's no Tower but we have something selected unselect it
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

void GameWindow::draw(sf::RenderTarget& target, sf::RenderStates) const {
	
}

void GameWindow::render(sf::RenderWindow& window) {
	window.clear(sf::Color::Black);
	renderMap(window);
	renderSelected(window);

	for (unsigned int i = 0; i < _map.objects.size(); ++i) {
		window.draw(*_map.objects[i]);
	}
	for (unsigned int i = 0; i < emitters.size(); ++i) {
		window.draw(*emitters[i]);
	}
	Window::render(window);
}

void GameWindow::renderMap(sf::RenderWindow& window) {
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

		window.draw(s);
		window.draw(c);
	}
}

void GameWindow::renderSelected(sf::RenderWindow& window) {
	// Don't draw anything if nothing is selected
	if (_selected == nullptr) {
		return;
	}

	// Draw range of the object
	sf::CircleShape r(_selected->getRange());
	r.setPosition(_selected->getX() - _selected->getRange(),
		_selected->getY() - _selected->getRange());

	r.setFillColor(sf::Color(127, 255, 127, 86));
	window.draw(r);

	// Tower is shooting at something? Draw on top of range
	if (_selected->getTarget() != nullptr) {
		sfLine l(sf::Vector2f(_selected->getX(), _selected->getY()),
			sf::Vector2f(_selected->getTarget()->getX(),
			_selected->getTarget()->getY()), 1, sf::Color(255, 0, 0, 128));

		window.draw(l);
	}

	// Drawing perks

	// Box we'll be drawing everything from
	sf::RectangleShape box(sf::Vector2f(PERK_BOX_WIDTH, getHeight()));

	// Draw background box
	box.setFillColor(sf::Color(112, 112, 112));
	box.setOutlineColor(sf::Color(200, 200, 200));
	box.setOutlineThickness(2); // Defaults to 0, or no outline
	box.setPosition(getWidth() - PERK_BOX_WIDTH, 0);
	window.draw(box);

	// Draw each perk, with stack count and progress bar
	for (unsigned int i = 0; i < _selected->perkCount(); ++i) {
		// Max of 8 perks
		box.setSize(sf::Vector2f(PERK_BOX_WIDTH, getHeight() / 8));
		box.setFillColor(sf::Color(64, 64, 64));
		box.setPosition(getWidth() - PERK_BOX_WIDTH, (getHeight() / 8) * i);

		// Draw we'll draw the name to
		sf::Text text(_selected->getPerk(i)->getTitle(), _font);
		text.setPosition(box.getPosition());
		text.setColor(sf::Color(255, 255, 255));
		window.draw(box);
		window.draw(text);

		// Draw red background for duration bar
		box.setFillColor(sf::Color::Red);
		// Bar fills up half of the box, so 1/16 is half a box
		box.setSize(sf::Vector2f(PERK_BOX_WIDTH, getHeight() / 16));
		box.setPosition(getWidth() - PERK_BOX_WIDTH,
			(getHeight() / 16) + ((getHeight() / 8) * i));
		window.draw(box);

		// Draw green background for duration bar
		float percent = _selected->getPerk(i)->getDuration() /
			_selected->getPerk(i)->getMaxDuration();
		box.setFillColor(sf::Color::Green);
		box.setSize(sf::Vector2f(PERK_BOX_WIDTH * percent, getHeight() / 16));
		window.draw(box);
	}
}
