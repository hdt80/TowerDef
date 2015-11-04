#include "Projectile.h"

#include "Enemy.h"
#include "Tower.h"

#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Projectile::Projectile(Map* map, Enemy* e, Tower* t) :
	Object(map, t->getX(), t->getY(), 1, t->getSpeed()),
	_damage(t->getDamage()) {

	_target = e;
	_enemy = e;
}

Projectile::~Projectile() {

}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Projectile::onHit() {
	_toRemove = true;
	_enemy->applyDamage(_damage);
}

void Projectile::update(int diff) {
	// If the enemy we're going to is removed why update?
	if (_enemy->isToRemove()) {
		_toRemove = true;
		return;
	}
	move(diff);

	double deltaMove = (double)(_speed) * 0.000001f * diff;
	if (distanceWith(_enemy) < deltaMove * 2) {
		onHit();
	}
}