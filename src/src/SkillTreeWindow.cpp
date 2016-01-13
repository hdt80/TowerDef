#include "SkillTreeWindow.h"

#include "SkillTree.h"
#include "Logger.h"

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

}

void SkillTreeWindow::mouseEvent(sf::Event& e) {
    CORE_INFO("(%g, %g)", e.mouseMove.x, e.mouseMove.y);
}

void SkillTreeWindow::render(sf::RenderWindow& window) {
    if (_tree) {
        window.draw(*_tree);
    } else {
        CORE_WARNING("Tried to draw %x! An incomplete tree", _tree);
    }
}
