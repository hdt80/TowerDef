#include "LuaScript.h"

#include "Logger.h"
#include "Object.h"
#include "Map.h"
#include "Tower.h"
#include "Target.h"

LuaScript::LuaScript(bool defineClasses) {
	_loaded = false;
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table,
		sol::lib::package, sol::lib::math, sol::lib::debug);

	if (defineClasses) {
		CORE_INFO("Defining classes for %x", this);
		defineTarget();
		defineEnemy();
		defineTower();
		defineObject();
		defineMap();
		defineStats();
	}
}

void LuaScript::loadScript(const std::string& name) {
	lua.open_file(name);
	setLoaded(true);
}

void LuaScript::defineTower() {
	CORE_INFO("Defining Tower");
	sol::constructors<sol::types<Map*, float, float, Stats>> towerCon;
	sol::userdata<Tower> towerUserData (
		"Tower", towerCon,
		// Target methods
		"getX", &Tower::getX,
		"getY", &Tower::getY,
		//"distanceWith", &Tower::distanceWith,
		//"distanceWithSqr", &Tower::distanceWithSqr,
		"isSimpleTarget", &Tower::isSimpleTarget,
		// Object methods
		"contains", &Tower::contains,
		"applyStat", &Tower::applyStat,
		"setStats", &Tower::setStats,
		"getSpeed", &Tower::getSpeed,
		"getRange", &Tower::getRange,
		"getFireRate", &Tower::getFireRate,
		"getDamage", &Tower::getDamage,
		"getAccel", &Tower::getAccel,
		// Tower methods
		"getProjectile", &Tower::getProjectile,
		"setProjectile", &Tower::setProjectile
	);
	lua.set_userdata(towerUserData);
}

void LuaScript::defineObject() {
	CORE_INFO("Defining Object");
}

void LuaScript::defineTarget() {
	CORE_INFO("Defining Target");
	// Define the constructor used by a Target
	sol::constructors<sol::types<float, float>> targetConstuctor;
	// Create the user_data so we can call Target's methods within Lua
	sol::userdata<Target> targetUserData(
		"Target", targetConstuctor,
		"getX", &Target::getX,
		"getY", &Target::getY,
		"setPosition", &Target::setPosition,
		"isSimpleTarget", &Target::isSimpleTarget
	);
	lua.set_userdata(targetUserData);
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
