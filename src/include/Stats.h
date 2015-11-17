#ifndef _STATS_H
#define _STATS_H

class Stats {
public:
	Stats();

	// Negative operator
	Stats operator- () const;
	Stats operator+ (const Stats& s) const;
	void operator+=(const Stats& s);

	void print();

	int range;    	// Range in pixels
	float fireRate; // Time between each shot
	float damage;   // Damage of one shot
	int speed;		// How fast the bullets move
};

#endif