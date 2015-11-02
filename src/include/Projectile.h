#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "Object.h"

class Tower;
class Enemy;

class Projectile : public Object {
public:
	Projectile(Map* map, Enemy* e, Tower* t);
	~Projectile();

	// Called when the projectile hits their target
	void onHit();

	void update(int diff);
protected:
	float _damage;

	Enemy* _enemy;
};

#endif