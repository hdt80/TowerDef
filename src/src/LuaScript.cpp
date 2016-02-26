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
		defineTarget();
		defineObject();
		defineTower();
		defineEnemy();
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
	sol::usertype<Tower> towerUserData (
		"Tower", towerCon,
		// Target methods
		"getX", &Tower::getX,
		"getY", &Tower::getY,
		"setPosition", &Tower::setPosition,
		//"distanceWith", &Tower::distanceWith, // Doesn't work
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
		"getProjSpeed", &Tower::getProjSpeed,
		"setRange", &Tower::setRange,
		"setFireRate", &Tower::setFireRate,
		"setDamage", &Tower::setDamage,
		"setSpeed", &Tower::setSpeed,
		"setAccel", &Tower::setAccel,
		"setProjSpeed", &Tower::setProjSpeed,
		// Tower methods
		"getProjectile", &Tower::getProjectile,
		"setProjectile", &Tower::setProjectile
	);
	lua.set_usertype(towerUserData);
}

void LuaScript::defineObject() {
	sol::constructors<sol::types<>> objCon;
	sol::usertype<Object> objectUserData (
		"Object", objCon,
		"getX", &Object::getX,
		"getY", &Object::getY,
		// Object methods
		"contains", &Object::contains,
		"applyStat", &Object::applyStat,
		"setStats", &Object::setStats,
		"getSpeed", &Object::getSpeed,
		"getRange", &Object::getRange,
		"setRange", &Object::setRange
	);
	lua.set_usertype(objectUserData);
}

void LuaScript::defineTarget() {
	// Define the constructor used by a Target
	sol::constructors<sol::types<float, float>> targetConstuctor;
	// Create the user_data so we can call Target's methods within Lua
	sol::usertype<Target> targetUserData(
		"Target", targetConstuctor,
		"getX", &Target::getX,
		"getY", &Target::getY,
		"setPosition", &Target::setPosition,
		"isSimpleTarget", &Target::isSimpleTarget
	);
	lua.set_usertype(targetUserData);
}

void LuaScript::defineMap() {

}

void LuaScript::defineStats() {
	sol::constructors<sol::types<bool>,
		sol::types<float, float, float, float, float, float>> statsCon;
	sol::usertype<Stats> statsUserData (
		"Stats", statsCon
	);
	lua.set_usertype(statsUserData);
}

void LuaScript::defineEnemy() {
	sol::constructors<sol::types<>> enemyCon;
		//sol::types<Map*, float, Stats, Path*, int>> enemyCon;
	sol::usertype<Enemy> enemyUserData (
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
	lua.set_usertype(enemyUserData);
}
