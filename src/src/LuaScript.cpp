#include "LuaScript.h"

#include "Logger.h"
#include "Object.h"
#include "Map.h"

LuaScript::LuaScript(bool defineClasses) {
	lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::package, 
		sol::lib::math, sol::lib::debug);

	if (defineClasses) {
		addObject();
	}
}

void LuaScript::loadScript(const std::string& name) {
	lua.open_file(name);
	setLoaded(true);
}

void LuaScript::addObject() {
	sol::constructors<sol::types<Map*, float, float, int, Stats>> objectConst;

	sol::userdata<Object> objectUserData (
		"Object", objectConst,
		"getX", &Object::getX
	);
}
