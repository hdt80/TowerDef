#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "Object.h"
#include "Color.h"

class Tower;
class Enemy;

class Projectile : public Object {
public:
	Projectile(Map* map, Enemy* e, Tower* t, Color c);
	~Projectile();

	// Called when the projectile hits their target
	void onHit();

	void update(int diff);
protected:
	float _damage;
	Color _color;

	Enemy* _enemy;
};

#endif