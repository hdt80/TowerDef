#ifndef _MENU_H
#define _MENU_H

#include <string>
#include <map>

class Menu {
public:
	Menu();
	Menu(Menu* parent);
	~Menu();

	void addOption();
protected:
	// Each option will open a new Menu, if 
	std::map<std::string, Menu*> options;
};

#endif