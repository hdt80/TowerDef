#include "Enemy.h"
#include "Map.h"
#include "ParticleEmitter.h"
#include "Common.h"

#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////
// Constuctor and deconstrctor
///////////////////////////////////////////////////////////////////////////////
// Enemy::Enemy() : Target(0, 0) {
// 	Enemy(0, 0, nullptr);
// }

Enemy::Enemy(Map* map, float health, Stats s, Path* p, int collRadius) :
	Object(map, 0, 0, collRadius, s),
	_health(health), _maxHealth(health), _path(p), _pathPoint(0) {

	_shape.setRadius(ENEMY_WIDTH);
	_shape.setFillColor(sf::Color(255, 0, 0));

	_hpBar.setFillColor(sf::Color::Green);

	setPosition(p->getPoint(0)->X, p->getPoint(0)->Y);
	_target = new Target(p->getPoint(1));
}

Enemy::~Enemy() {
	if (_ended) {
		_map->setHealthRelative(-1);
	}
	ParticleEmit::emit(x, y, 50, Color(255, 0, 0, 255));
}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(_shape);
	target.draw(_hpBar);
}

void Enemy::update(int diff) {
	move(diff);

	// We aren't moving anywhere, move back to 0, 0
	if (_target == nullptr) {
		_direction = Vector2();
		return;
	}

	// Update shape positions
	_shape.setPosition(getX() - ENEMY_WIDTH, getY() - ENEMY_WIDTH);
	_hpBar.setPosition(getX() - ENEMY_WIDTH, getY() - ENEMY_WIDTH - 6);

	// Calc how far they've should have moved since the last update
	double deltaMove = (double)getSpeed() * 0.000001f * diff;

	// If we've reached the end of our target
	if (_target->isSimpleTarget() && distanceWith(_target) < deltaMove * 2) {
		// If there is still points in the path left4
		if (++_pathPoint >= _path->size()) {
			_ended = true;
			_toRemove = true;
			setTarget(nullptr);
		} else {
			// Start moving to the next point
			setTarget(new Target(_path->getPoint(_pathPoint)));
		}
	}
}

// Positive damage values take health away while negative values add health
void Enemy::applyDamage(float amount) {
	_health -= amount;
	_hpBar.setSize(sf::Vector2f(
		ENEMY_WIDTH * 2 * (getHealth() / getMaxHealth()), 4));
	if (_health <= 0) {
		_toRemove = true;
	}
}