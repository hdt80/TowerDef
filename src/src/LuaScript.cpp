#include "LuaScript.h"

#include "Logger.h"
#include "Object.h"
#include "Map.h"
#include "Tower.h"
#include "Target.h"
#include "Enemy.h"

LuaScript::LuaScript(bool defineClasses) {
	_loaded = false;
	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table,
		sol::lib::package, sol::lib::math, sol::lib::debug);

	if (defineClasses) {
		CORE_INFO("Defining classes for %x", this);
		defineTarget();
		defineTower();
		defineEnemy();
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
	sol::constructors<sol::types<>> objCon;
	sol::userdata<Object> objectUserData (
		"Object", objCon,
		"getX", &Object::getX,
		"getY", &Object::getY,
		// Object methods
		"contains", &Object::contains,
		"applyStat", &Object::applyStat,
		"setStats", &Object::setStats,
		"getSpeed", &Object::getSpeed,
		"getRange", &Object::getRange
	);
	lua.set_userdata(objectUserData);
}

void LuaScript::defineTarget() {
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

}

void LuaScript::defineStats() {

}

void LuaScript::defineEnemy() {
	sol::constructors<sol::types<>> enemyCon;
		//sol::types<Map*, float, Stats, Path*, int>> enemyCon;
	sol::userdata<Enemy> enemyUserData (
		"Enemy", enemyCon,
		// Target methods
		"getX", &Enemy::getX,
		"getY", &Enemy::getY,
		//"distanceWith", &Enemy::distanceWith,
		//"distanceWithSqr", &Enemy::distanceWithSqr,
		"isSimpleTarget", &Enemy::isSimpleTarget,
		// Object methods
		"contains", &Enemy::contains,
		"applyStat", &Enemy::applyStat,
		"setStats", &Enemy::setStats,
		"getSpeed", &Enemy::getSpeed,
		"getRange", &Enemy::getRange,
		"getFireRate", &Enemy::getFireRate,
		"getDamage", &Enemy::getDamage,
		"getAccel", &Enemy::getAccel,
		// Enemy methods
		"applyDamage", &Enemy::applyDamage,
		"getHealth", &Enemy::getHealth,
		"getMaxHealth", &Enemy::getMaxHealth,
		"getPath", &Enemy::getPath,
		"setHealth", &Enemy::setHealth,
		"setMaxHealth", &Enemy::setMaxHealth,
		"setTarget", &Enemy::setTarget,
		"setPath", &Enemy::setPath
	);
	lua.set_userdata(enemyUserData);
}
