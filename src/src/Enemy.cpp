#include "Enemy.h"

#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////
// Constuctor and deconstrctor
///////////////////////////////////////////////////////////////////////////////
Enemy::Enemy() : Target(0, 0) {
	Enemy(0, 0, nullptr);
}

Enemy::Enemy(int health, int speed, Path* p) :
	Target(0, 0), _health(health), _maxHealth(health), _speed(speed), 
	_target(nullptr), _path(p), _pathPoint(0), _ended(false) {

	setPosition(p->getPoint(0)->X, p->getPoint(0)->Y);
	_target = new Target(p->getPoint(1));
}

Enemy::~Enemy() {

}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Enemy::update(int diff) {
	// We aren't moving anywhere, move back to 0, 0
	if (_target == nullptr) {
		direction = Vector2();
		return;
	}

	Vector2 to(_target->getX(), _target->getY());
	Vector2 cur(x, y);

	Vector2 goingTo(to - cur);
	direction = goingTo.normalize();

	// Calc how far they've should have moved since the last update
	double deltaMovement = (double)(_speed) * 0.000001f * diff;

	float xx = direction.X * deltaMovement;
	float yy = direction.Y * deltaMovement;

	x += xx;
	y += yy;

	if (_target->isSimpleTarget() &&
		distanceWith(_target) < deltaMovement * 2) {
		// They're at the end
		if (++_pathPoint >= _path->size()) {
			setEnded(true);
			setTarget(nullptr);
		} else {
			// Not at end? Move to next point
			setTarget(new Target(_path->getPoint(_pathPoint)));
		}

	}
}