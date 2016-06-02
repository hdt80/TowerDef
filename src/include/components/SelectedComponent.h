#ifndef SELECTED_COMPONENT_H
#define SELECTED_COMPONENT_H

#include "GuiComponent.h"
#include <memory>

class Tower;
class GameWindow;

class SelectedComponent : public GuiComponent {
public:
	SelectedComponent();
	SelectedComponent(GameWindow* window);
	~SelectedComponent();

	void update(int diff);
	void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	const Tower* _selected;
};

#endif
