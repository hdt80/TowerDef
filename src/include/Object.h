#ifndef _OBJECT_H
#define _OBJECT_H

#include "Target.h"

class Map;

class Object : public Target {
public:
	//Object();
	Object(Map* map, float x, float y, int collRadius, int speed = 0);
	virtual ~Object();

	virtual void onCollision(Object* o);
	bool collidesWith(Object* o);

	// Moves the object depending on it's target, updating it's position
	// diff - Milliseconds the object is supposed to move for
	virtual void move(int diff);

	// Update the enemy depending on its target, updating its coords
	// diff - Milliseconds since last update call
	virtual void update(int diff);

	// A simple target is just an (x, y) coord point. Because an Object
	// isn't just a coord point it isn't a simple target
	virtual bool isSimpleTarget() { return false; }
	virtual int getSpeed() { return 0; }

	Map* getMap() const { return _map; }
	Target* getTarget() const { return _target; }
	Vector2 getDirection() const { return _direction; }
	bool isToRemove() const { return _toRemove; }
	int getCollisionRadius() const { return _collisionRadius; }

	void setTarget(Target* t);
	void setToRemove(bool b) { _toRemove = b; }

	virtual void setPosition(float x, float y);

protected:
	Map* _map; // Map this object is located on

	Target* _target; // Target the enemy is running to (can be coord or enemy)
	Vector2 _direction; // Direction they're moving to

	bool _toRemove; // Is this object marked for removal?
	int  _collisionRadius;
	int  _speed; // Pixels per second
};

#endif