#ifndef _STATS_H
#define _STATS_H

class Stats {
public:
	Stats();

	int range;    // Range in pixels
	float fireRate; // Time between each shot
	float damage;   // Damage of one shot
	int speed;		// How fast the bullets move
};

#endif