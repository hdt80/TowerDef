#ifndef _LUA_SCRIPT_H
#define _LUA_SCRIPT_H

#include <string>
#include "lua/selene.h"
#include "sol/sol.hpp"

class LuaScript {
public:
	LuaScript(bool loadedClasses = true);

	bool isLoaded() { return _loaded; }
	void setLoaded(bool b) { _loaded = b; }

	void loadScript(const std::string& name);

	sol::state lua;

protected:
	bool _loaded;

	void defineEnemy();
	void defineTower();
	void defineObject();
	void defineTarget();
	void defineMap();
	void defineStats();
};

#endif
