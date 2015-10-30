#ifndef _ENEMY_H
#define _ENEMY_H

#include "Path.h"
#include "Vector2.h"
#include "Object.h"

class Enemy : public Object {
public:
	//Enemy();
	Enemy(Map* map, int health, int speed, Path* p, int collRadius);
	virtual ~Enemy();

	// Update the enemy depending on its target, updating its coords
	// @param diff - Amounts of milliseconds the object is supposed to move
	virtual void update(int diff);
	virtual bool isSimpleTarget() { return false; }

	int getHealth() { return _health; }
	int getMaxHealth() { return _maxHealth; }
	Path* getPath() { return _path; }

	void setHealth(int health) { this->_health = health; }
	void setMaxHealth(int h) { this->_maxHealth = h; }
	void setTarget(Target* t) { this->_target = t; }
	void setPath(Path* p) { this->_path = p; }

protected:
	int _health;	// Current health of an enemy
	int _maxHealth;	// Max health of the enemy

	Path* _path;	// Path the enemy will follow
	unsigned int _pathPoint;// Index in path of the point it's travelling to
};

#endif