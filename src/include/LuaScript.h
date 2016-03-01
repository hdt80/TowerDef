#ifndef _LUA_SCRIPT_H
#define _LUA_SCRIPT_H

#include <string>
#include "sol/sol.hpp"
#include "Logger.h"

class LuaScript {
public:
	LuaScript(bool loadedClasses = true);

	bool isLoaded() { return _loaded; }
	void setLoaded(bool b) { _loaded = b; }

	void loadScript(const std::string& name);

	template<typename... Args>
	void callFunction(const char* name, Args&&... args) {
		if (isLoaded()) {
			try {
				lua.get<sol::function>(name).template call<void>(args...);
			} catch (sol::error e) {
				CORE_ERROR("[Lua Scripts %x] %s", this, e.what());
			}
		}
	}

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
