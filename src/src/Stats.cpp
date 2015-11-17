#include "Stats.h"

#include "Logger.h"

Stats::Stats() {
	range = 0;
	fireRate = 0.0f;
	damage = 0.0f;
	speed = 0;
}

Stats Stats::operator- () const {
	Stats s;
	s.range    = -range;
	s.fireRate = -fireRate;
	s.damage   = -damage;
	s.speed    = -speed;
	return s;
}

Stats Stats::operator+ (const Stats& o) const {
	Stats s;
	s.range    = range + o.range;
	s.fireRate = fireRate + o.fireRate;
	s.damage   = damage + o.damage;
	s.speed    = speed + o.speed;
	return s;
}

void Stats::operator+= (const Stats& o) {
	*this = *this + o;
}

void Stats::print() {
	CORE_INFO("Range: %i", range);
	CORE_INFO("FireRate: %f", fireRate);
	CORE_INFO("Damage: %f", damage);
	CORE_INFO("speed: %f", speed);
}