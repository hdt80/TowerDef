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

void LuaScript::loadScript(const std::string& path) {
	try {
		_name = path;
		lua.script_file(path);
		_loaded = true;
	} catch (sol::error e) {
		setLoaded(false);
		CORE_ERROR("[Lua Script %x] Error when loading from \"%s\": %s",
			this, path.c_str(), e.what());
	}
}

void LuaScript::printTable() {
	auto iter = lua.begin();
	CORE_INFO("[LuaScript %x] Print %s", this, _name.c_str());
	while (iter != lua.end()) {
		CORE_INFO("%s", (*iter).first.as<std::string>().c_str());
		++iter;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Object defenitions for Lua scripts
///////////////////////////////////////////////////////////////////////////////
void LuaScript::defineTower() {
	lua.new_usertype<Tower> (
		"Tower", sol::constructors<sol::types<Map*, float, float, Stats>>(),
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
}

void LuaScript::defineObject() {
	lua.new_usertype<Object> (
		"Object", sol::constructors<
				sol::types<Map*, float, float, int, Stats>>(),
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
}

void LuaScript::defineTarget() {
	lua.new_usertype<Target> (
		"Target", sol::constructors<sol::types<float, float>>(),
		"getX", &Target::getX,
		"getY", &Target::getY,
		"setPosition", &Target::setPosition,
		"isSimpleTarget", &Target::isSimpleTarget
	);
}

void LuaScript::defineMap() {

}

void LuaScript::defineStats() {
	lua.new_usertype<Stats> (
		"Stats", sol::constructors<
			sol::types<bool>,
			sol::types<float, float, float, float, float, float, bool>
		>(),		
		"range", &Stats::range,
		"fireRate", &Stats::fireRate,
		"damage", &Stats::damage,
		"projSpeed", &Stats::projSpeed,
		"accel", &Stats::accel
	);
}

void LuaScript::definePerk() {
	lua.new_usertype<Perk> (
		"Perk", sol::constructors<sol::types<std::string, Stats, float, int>>(),
		"getName", &Perk::getName,
		"getTitle", &Perk::getTitle,
		"getStats", &Perk::getStats
	);
}

void LuaScript::defineEnemy() {
	lua.new_usertype<Enemy> (
		"Enemy", sol::constructors<
				sol::types<Map*, float, Stats, Path*, int>>(),
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
}
