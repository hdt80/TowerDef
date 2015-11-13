#ifndef _STATS_H
#define _STATS_H

class Stats {
public:
	Stats();

	// Negative operator
	// Return a Stats with the opposite values of the Stats
	// Used when removing a Perk, it'll just add the negative Stats
	// which since it's negative in the first place, it'll return positive
	// values that can be added to get it's original value
	Stats operator- () const {
		Stats s;
		s.range    = -range;
		s.fireRate = -fireRate;
		s.damage   = -damage;
		s.speed    = -speed;
		return s;
	}

	int range;    	// Range in pixels
	float fireRate; // Time between each shot
	float damage;   // Damage of one shot
	int speed;		// How fast the bullets move
};

#endif