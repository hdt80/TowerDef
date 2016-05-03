#ifndef _STATS_H
#define _STATS_H

class Stats {
public:
	Stats(bool perc = false);
    Stats(float range, float fireRate, float damage, float projSpeed,
        float speed, float accel, bool perc = false) :
		range(range), fireRate(fireRate), damage(damage), projSpeed(projSpeed),
		speed(speed), accel(accel), percent(perc) {};

	// Negative operator
	Stats operator- () const;

	Stats operator+ (const Stats& s) const;
	void operator+=(const Stats& s);
    Stats operator* (float m) const;

	void print();

	float range;   	// Range in pixels
	float fireRate; // Time between each shot
	float damage;   // Damage of one shot
	float projSpeed;// Projectile speed, pixels per second
	float speed;	// Objects speed, pixels per second
	float accel;	// How much the speed will change each second

    //A percentage Stat is used by Perks. Percent stats take base stats
    //To calculate new values.
    //Eg. range(500) * range(0.5) = 250
    //With a base stat of 500 range a 0.5 (50%) increase will add 250 range
    //to make 500 range
    bool percent;

};

#endif
