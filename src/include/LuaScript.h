#ifndef _LUA_SCRIPT_H
#define _LUA_SCRIPT_H

#include <string>
#include "lua/selene.h"

class LuaScript {
public:
	LuaScript();

	bool isLoaded() { return _loaded; }
	void setLoaded(bool b) { _loaded = b; }

	void loadScript(const std::string& name);

	sel::State lua;

protected:
	bool _loaded;
};

#endif
