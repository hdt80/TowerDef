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

protected:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	Object _projectile; // What we'll be shooting at our _target

	// Microseconds now that we shot.
	// If we shot one second ago _lastShot will be 1,000,000
	// If fireRate is >= to _lastShot you can shoot
	float _lastShot;
};

#endif
