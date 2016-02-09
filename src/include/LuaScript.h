#ifndef _LUA_SCRIPT_H
#define _LUA_SCRIPT_H

#include <string>
#include "sol/sol.hpp"
class LuaScript {
public:
	LuaScript(bool defineClasses = false);

	bool isLoaded() { return _loaded; }
	void setLoaded(bool b) { _loaded = b; }

	void loadScript(const std::string& name);

	sol::table getTable(const std::string& name);

	sol::state lua;

protected:
	bool _loaded;

private:
	void addObject();
};

#endif
