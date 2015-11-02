#ifndef _ENEMY_H
#define _ENEMY_H

#include "Path.h"
#include "Vector2.h"
#include "Object.h"

class Enemy : public Object {
public:
	//Enemy();
	Enemy(Map* map, float health, int speed, Path* p, int collRadius);
	virtual ~Enemy();

	// Update the enemy depending on its target, updating its coords
	// @param diff - Amounts of milliseconds the object is supposed to move
	virtual void update(int diff);
	virtual bool isSimpleTarget() { return false; }

	float getHealth() { return _health; }
	float getMaxHealth() { return _maxHealth; }
	Path* getPath() { return _path; }

	void setHealth(float health) { this->_health = health; }
	void setMaxHealth(float h) { this->_maxHealth = h; }
	void setTarget(Target* t) { this->_target = t; }
	void setPath(Path* p) { this->_path = p; }

protected:
	float _health;	// Current health of an enemy
	float _maxHealth;	// Max health of the enemy

	Path* _path;	// Path the enemy will follow
	unsigned int _pathPoint;// Index in path of the point it's travelling to

	bool _ended; // If this enemy reached the end of the path
};

#endif