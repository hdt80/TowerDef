#include "Tower.h"

#include "Logger.h"
#include <vector>
#include "Map.h"
#include "Enemy.h"

///////////////////////////////////////////////////////////////////////////////
// Constuctor and deconstrctor
///////////////////////////////////////////////////////////////////////////////
Tower::Tower(Map* map, float x, float y) : Object(map, x, y, 20, 0), 
	_lastShot(0.0f) {

}

Tower::~Tower() {
	
}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////

void Tower::update(int diff) {
	_lastShot += diff * 0.000001f;
	if (_lastShot > _stats.fireRate) {
		_lastShot = 0;
	}

	// Find the nearest target if we don't have one
	if (_target == nullptr) {
		std::vector<Enemy*>& enemies = _map->enemies;
		for (unsigned int i = 0; i < enemies.size(); ++i) {
			if (distanceWith(enemies[i]) < getRange()) {
				_target = enemies[i];
				break;
			}
		}
	} else {
		// _target is a Target, and not an object, therefore not having
		// _toRemove, which we need to check to see if _target is going to
		// be deleted next update. 
		// _target should always be an enemy, but to be safe I check it
		Object* o = nullptr;
		if (!_target->isSimpleTarget()) {
			o = static_cast<Object*>(_target);
		}
		if (o == nullptr) {
			// If the cast didn't work, and _target isn't an Object just
			// check the range between the two points. This should never occur
			CORE_WARNING("Tower[%i] isn't targetting an enemy[%i]", this, _target);
			if (distanceWith(_target) > getRange()) {
				_target = nullptr;
			}
		} else {
			// If the cast did work, which it always should, check if it's 
			// going to be removed next update. If it is we should find a
			// new _target
			if (distanceWith(o) > getRange() || o->isToRemove()) {
			_target = nullptr;
			}
		}
	}
}