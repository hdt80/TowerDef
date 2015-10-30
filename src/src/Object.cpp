#include "Object.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Object::Object(Map* map, float x, float y, int collRadius, int speed) :
	Target(x, y), _map(map), _target(nullptr), _toRemove(false),
	_collisionRadius(collRadius), _speed(speed) {

}

Object::~Object() {}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Object::onCollision(Object* o) {

}

bool Object::collidesWith(Object* o) {

}

void Object::move(int diff) {
	// No need to calculate movement if they can't move
	if (_speed <= 0) {
		return;
	}
	// If we don't have anywhere we've moving to don't move
	if (_target == nullptr) {
		_direction = Vector2();
		return;
	}

	Vector2 to(_target->getX(), _target->getY());
	Vector2 cur(x, y);

	Vector2 goingTo(to - cur);
	_direction = goingTo.normalize();

	double deltaMove = (double)_speed * 0.000001f * diff;

	x += _direction.X * deltaMove;
	y += _direction.Y * deltaMove;
}

void Object::update(int diff) {

}


///////////////////////////////////////////////////////////////////////////////
// Getters and setters
///////////////////////////////////////////////////////////////////////////////
void Object::setTarget(Target* t) {
	// Are we already targetting the target?
	if (_target == t) {
		return;
	}

	// If we aren't targetting the same target we check if we need to delete
	// the old target. If it's a simple target we can delete it safely, but if
	// it isn't we will be deleting an object that is in use
	if (_target && _target->isSimpleTarget()) {
		delete _target;
	}
	_target = t;
}

void Object::setPosition(float nx, float ny) {
	x = nx;
	y = ny;
	setTarget(nullptr);
}