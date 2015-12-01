#include "Tower.h"

#include "Logger.h"
#include "Map.h"
#include "Enemy.h"
#include "Color.h"
#include "Common.h"

#include <vector>

///////////////////////////////////////////////////////////////////////////////
// Constuctor and deconstrctor
///////////////////////////////////////////////////////////////////////////////
Tower::Tower(Map* map, float x, float y, Stats s) : Object(map, x, y, 20, s), 
	_lastShot(0.0f) {

	_shape.setRadius(TOWER_WIDTH);
	_shape.setFillColor(sf::Color(0, 255, 0));

	// Some towers used as projectiles can move, but if it can't, only set
	// its shape's position once
	if (getSpeed() <= 0.0f) {
		_shape.setPosition(getX() - TOWER_WIDTH, getY() - TOWER_WIDTH);
	}
	CORE_INFO("I'm %x (tower)", this);
}

Tower::~Tower() {
	CORE_INFO("WHAT?");
}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Tower::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(_shape);
}

void Tower::update(int diff) {
	_lastShot += diff * 0.000001f;
	if (_lastShot > _stats.fireRate) {
		_lastShot = 0;
		shoot();
	}

	// Find the nearest target if we don't have one
	if (_target == nullptr) {
		std::vector<Object*>& enemies = _map->objects;
		for (unsigned int i = 0; i < enemies.size(); ++i) {
			if (isEnemy(enemies[i])) {
				if (distanceWith(enemies[i]) < getRange()) {
					_target = enemies[i];
					break;
				}
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
			CORE_WARNING("Tower[%i] isn't firing an enemy[%i]", this, _target);
			if (distanceWith(_target) > getRange()) {
				delete _target;
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
	Object::update(diff);
}

void Tower::shoot() {
	if (_target == nullptr) {
		return;
	}

	Enemy* e = nullptr;
	if (!_target->isSimpleTarget()) {
		e = static_cast<Enemy*>(_target);
		Projectile* p = new Projectile(_map, e, this, Color(127, 127, 127, 255));
		_map->shoot(this, p);
	} else {
		CORE_WARNING("Failed to shoot at %i", _target);
	}
}