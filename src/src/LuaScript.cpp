#include "LuaScript.h"

#include "Logger.h"
#include "Object.h"
#include "Map.h"
#include "Tower.h"
#include "Target.h"

LuaScript::LuaScript(bool defineClasses) {
	_loaded = false;

	if (defineClasses) {
		CORE_INFO("Defining classes for %x", this);
		defineEnemy();
		defineTower();
		defineObject();
		defineMap();
		defineStats();
		defineTarget();
	}
}

void LuaScript::loadScript(const std::string& name) {
	setLoaded(lua.Load(name));
}

void LuaScript::defineTower() {
	CORE_INFO("Defining Tower");
	lua["Tower"].SetClass<Tower, Map*, float, float, Stats>(
		"getProjectile", &Tower::getProjectile,
		"setProjectile", &Tower::setProjectile
	);
}

void LuaScript::defineObject() {
	CORE_INFO("Defining Object");
	lua["Object"].SetClass<Object, Map*, float, float, int, Stats>(
		
	);
}

void LuaScript::defineTarget() {
	CORE_INFO("Defining Target");
	lua["Target"].SetClass<Target, float, float> (
		//"distanceWith", &Target::distanceWith,
		//"distanceWithSqr", &Target::distanceWithSqr,
		"getX", &Target::getX
		//"getY", &Target::getY
		//"setPosition", &Target::setPosition
	);
}

void LuaScript::defineMap() {
	CORE_INFO("Defining Map");

}

void LuaScript::defineStats() {
	CORE_INFO("Defining Stats");

}

void LuaScript::defineEnemy() {
	CORE_INFO("Defining Enemy");

}
