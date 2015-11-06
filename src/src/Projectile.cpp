#include "Projectile.h"

#include "Enemy.h"
#include "Tower.h"
#include "ParticleEmitter.h"

#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Projectile::Projectile(Map* map, Enemy* e, Tower* t, Color c) :
	Object(map, t->getX(), t->getY(), 1, *t->getStats()),
	_color(c) {

	_target = e;
	_enemy = e;
}

Projectile::~Projectile() {

}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Projectile::onHit() {
	ParticleEmit::emit(x, y, 10, _color);
	_toRemove = true;
	_enemy->applyDamage(getDamage());
}

void Projectile::update(int diff) {
	// If the enemy we're going to is removed why update?
	if (_enemy->isToRemove()) {
		_toRemove = true;
		return;
	}
	move(diff);

	double deltaMove = (double)getSpeed() * 0.000001f * diff;
	if (distanceWith(_enemy) < deltaMove * 2) {
		onHit();
	}
}