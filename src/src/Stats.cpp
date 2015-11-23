#include "Stats.h"

#include "Logger.h"

Stats::Stats() {
	range = 0;
	fireRate = 0.0f;
	damage = 0.0f;
	speed = 0.0f;
	projSpeed = 0.0f;
	accel = 0.0f;
}

Stats Stats::operator- () const {
	Stats s;
	s.range    = -range;
	s.fireRate = -fireRate;
	s.damage   = -damage;
	s.speed    = -speed;
	s.accel	   = -accel;
	s.projSpeed = -projSpeed;
	return s;
}

Stats Stats::operator+ (const Stats& o) const {
	Stats s;
	s.range    = range + o.range;
	s.fireRate = fireRate + o.fireRate;
	s.damage   = damage + o.damage;
	s.speed    = speed + o.speed;
	s.accel	   = accel + o.accel;
	s.projSpeed= projSpeed + o.projSpeed;
	return s;
}

void Stats::operator+= (const Stats& o) {
	*this = *this + o;
}

void Stats::print() {
	CORE_INFO("Range: %i", range);
	CORE_INFO("FireRate: %f", fireRate);
	CORE_INFO("Damage: %f", damage);
	CORE_INFO("Speed: %f", speed);
	CORE_INFO("Projectile Speed: %f", projSpeed);
	CORE_INFO("Accel: %f", accel);
}