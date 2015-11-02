#ifndef _TOWER_H
#define _TOWER_H

#include "Target.h"
#include "Object.h"
#include "TowerStats.h"

class Tower : public Object {
public:
	Tower(Map* map, float x, float y);
	~Tower();

	void update(int diff);

	void setRange(int range) { _stats.range = range; }
	void setFireRate(float r) { _stats.fireRate = r; }
	void setDamage(float d) { _stats.damage = d; }

	int getRange() { return _stats.range; }
	float getFireRate() { return _stats.fireRate; }
	float getDamage() { return _stats.damage; }

protected:
	TowerStats _stats;
};

#endif
