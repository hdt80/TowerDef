#ifndef _TARGET_H
#define _TARGET_H

#include "Vector2.h"

class Target {
public:
	Target(float x, float y) : x(x), y(y) {}
	Target(Vector2 v) : x(v.X), y(v.Y) {}
	Target(Vector2* v) : x(v->X), y(v->Y) {}
	virtual ~Target(){}

	float distanceWith(Target* t);
	float distanceWith(float dx, float dy);
	float distanceWithSqr(Target* t);
	float distanceWithSqr(float dx, float dy);

	float getX() const { return x; }
	float getY() const { return y; }

	void setPosition(float x, float y) {
		this->x = x;
		this->y = y;
	}

	virtual bool isSimpleTarget() { return true; }
protected:
	float x;
	float y;
};

#endif