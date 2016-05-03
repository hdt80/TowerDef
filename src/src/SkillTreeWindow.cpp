#include "SkillTreeWindow.h"

#include "SkillTree.h"
#include "Logger.h"
#include "Game.h"

SkillTreeWindow::SkillTreeWindow(SkillTree* tree, Vector2 size) {
	_size = size;
	_name = "Skill Tree";
	if (!tree) {
		CORE_WARNING("[%s] Recived null tree", _name.c_str());
	}
	_tree = tree;
}

SkillTreeWindow::~SkillTreeWindow() {
	
}

void SkillTreeWindow::init() {
    Window::init();
}

void SkillTreeWindow::update(int diff) {

}

void SkillTreeWindow::keyEvent(sf::Event& e) {
    if (e.key.code == sf::Keyboard::T) {
        Game::WindowManager.pop();
    } else if (e.key.code == sf::Keyboard::D) {
        _tree->print(_tree->getHead(), true);
    }
}

void SkillTreeWindow::mouseEvent(sf::Event& e) {
    SkillNode* node = _tree->getNode(e.mouseButton.x, e.mouseButton.y);
    node->print();
    if (node) {
        CORE_INFO("Clicked on \'%s\'", node->name().c_str());
        if (node->unlocked()) {
            node->incPoints();
			if (node->points > 0) {
				node->attached = _tree->_attached;
				_tree->_attached->addPerk(node->perk);
			}
        }
    }
}

void SkillTreeWindow::render(sf::RenderWindow& window) {
    if (_tree) {
        window.draw(*_tree);
    } else {
        CORE_WARNING("Tried to draw %x! An incomplete tree", _tree);
    }
}
