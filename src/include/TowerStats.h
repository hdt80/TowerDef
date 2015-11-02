#ifndef _TOWER_STATS_H
#define _TOWER_STATS_H

class TowerStats {
public:
	TowerStats();

	int range;    // Range in pixels
	float fireRate; // Shots per second
	float damage;   // Damage of one shot
};

#endif