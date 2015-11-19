#include "Projectile.h"

#include "Enemy.h"
#include "Tower.h"
#include "ParticleEmitter.h"
#include "Perk.h"
#include "Common.h"

#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Projectile::Projectile(Map* map, Enemy* e, Tower* t, Color c) :
	Object(map, t->getX(), t->getY(), 1, *t->getStats()),
	_color(c), _shooter(t) {

	_shape.setRadius(PROJECTILE_WIDTH);
	_shape.setFillColor(sf::Color(120, 120, 120));

	_target = e;
	_enemy = e;
}

Projectile::~Projectile() {

}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(_shape);
}

void Projectile::onHit() {
	ParticleEmit::emit(x, y, 10, _color);
	_enemy->applyDamage(getDamage());

	Stats* perk = new Stats;
	perk->fireRate = -(_shooter->getFireRate() / 2.0f);
	Perk* p = new Perk("AS", *perk, 3, 3);

	_shooter->addPerk(p);

	_toRemove = true;
}

void Projectile::update(int diff) {
	// If the enemy we're going to is removed why update?
	if (_enemy->isToRemove()) {
		_toRemove = true;
		return;
	}
	move(diff);
	_shape.setPosition(getX() - PROJECTILE_WIDTH, getY() - PROJECTILE_WIDTH);

	double deltaMove = (double)getSpeed() * 0.000001f * diff;
	if (distanceWith(_enemy) < deltaMove * 2) {
		onHit();
	}
}