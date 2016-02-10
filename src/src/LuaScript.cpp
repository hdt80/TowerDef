#include "LuaScript.h"

#include "Logger.h"
#include "Object.h"
#include "Map.h"

LuaScript::LuaScript() {

}

void LuaScript::loadScript(const std::string& name) {
	setLoaded(true);
}
