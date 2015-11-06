#ifndef _TOWER_H
#define _TOWER_H

#include "Target.h"
#include "Object.h"
#include "Stats.h"

class Tower : public Object {
public:
	Tower(Map* map, float x, float y, Stats s);
	~Tower();

	void update(int diff);
	// Shoot at the target
	void shoot();

	void setRange(int range) { _stats.range = range; }
	void setFireRate(float r) { _stats.fireRate = r; }
	void setDamage(float d) { _stats.damage = d; }
	void setSpeed(int s) { _stats.speed = s; }

	int getRange() const { return _stats.range; }
	float getFireRate() const { return _stats.fireRate; }
	float getDamage() const { return _stats.damage; }
	int getSpeed() const { return _stats.speed; }

protected:
	// Microseconds now that we shot.
	//If we shot one second ago _lastShot will be 1,000,000
	// If fireRate is >= to _lastShot you can shoot
	float _lastShot;
};

#endif
