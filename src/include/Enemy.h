#ifndef _ENEMY_H
#define _ENEMY_H

#include "Path.h"
#include "Target.h"
#include "Vector2.h"

class Map;

class Enemy : public Target {
public:
	Enemy();
	Enemy(int health, int speed);
	~Enemy();

	// Update the enemy depending on its target, updating its coords
	// @param diff - Amounts of milliseconds the object is supposed to move
	virtual void update(int diff);
	virtual bool isSimpleTarget() { return false; }

	int getHealth() { return _health; }
	int getMaxHealth() { return _maxHealth; }
	int getSpeed() { return _speed; }
	Target* getTarget() { return _target; }
	bool hasEnded() { return _ended; }

	void setHealth(int health) { this->_health = health; }
	void setMaxHealth(int h) { this->_maxHealth = h; }
	void setSpeed(int speed) { this->_speed = speed; }
	void setTarget(Target* t) { this->_target = t; }
	void setEnded(bool b) { this->_ended = b; }

protected:
	int _health;	// Current health of an enemy
	int _maxHealth;	// Max health of the enemy
	int _speed;		// Pixels per second

	Target* _target; // Target the enemy is running to (can be coord or enemy)
	Vector2 direction;

	Path* _path;	// Path the enemy will follow
	unsigned int _pathPoint;// Index in path of the point it's travelling to

	bool _ended;	// Enemy has ended it's pathing
};

#endif