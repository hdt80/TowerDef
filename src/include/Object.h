#ifndef _OBJECT_H
#define _OBJECT_H

#include "Target.h"

#include "Stats.h"

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Map;
class Perk;

class Object : public Target, public sf::Drawable, sf::Transformable {
public:
	Object();
	Object(Map* map, float x, float y, int collRadius, Stats s);
	virtual ~Object();

	virtual void onCollision(Object* o);
	bool collidesWith(Object* o);

	// If that point is within our collision box
	bool contains(float x, float y);
	virtual sf::FloatRect getCollisionBox() { return _shape.getGlobalBounds(); }

	// Moves the object depending on it's target, updating it's position
	// diff - Milliseconds the object is supposed to move for
	virtual void move(int diff);

	// Update the enemy depending on its target, updating its coords
	// diff - Milliseconds since last update call
	virtual void update(int diff);

	// A simple target is just an (x, y) coord point. Because an Object
	// isn't just a coord point it isn't a simple target
	virtual bool isSimpleTarget() { return false; }

	// Stats
	// Apply new stats to the object
	// If it's relative change stats relative to current stats
	void applyStat(Stats s, bool relative = true);
	// Stats getters
	Stats* getStats() { return &_stats; }
	int getSpeed() { return _stats.speed; }
	int getRange() const { return _stats.range; }
	float getFireRate() const { return _stats.fireRate; }
	float getDamage() const { return _stats.damage; }
	float getAccel() const { return _stats.accel; }
	float getProjSpeed() const { return _stats.projSpeed; }
	// Stats setters
	void setRange(int r) { _stats.range = r; }
	void setFireRate(float r) { _stats.fireRate = r; }
	void setDamage(float d) { _stats.damage = d; }
	void setSpeed(int s) { _stats.speed = s; }
	void setAccel(float f) { _stats.accel = f; }
	void setProjSpeed(float f) { _stats.projSpeed = f; }

	// Perk methods
	virtual void addPerk(Perk* p);
	virtual void removePerk(Perk* p);
	bool hasPerk(std::string name) { return getPerk(name) != nullptr; }
	Perk* getPerk(int index) { return _perks[index];}
	Perk* getPerk(std::string name); // nullptr if no Perk with that name

	// Other getters
	Map* getMap() const { return _map; }
	std::vector<Perk*>& getPerks() { return _perks; }
	Target* getTarget() const { return _target; }
	Vector2 getDirection() const { return _direction; }
	bool isToRemove() const { return _toRemove; }
	int getCollisionRadius() const { return _collisionRadius; }

	// Utility getters
	unsigned int perkCount() const { return _perks.size(); }

	// Other setters
	void setTarget(Target* t);
	void setToRemove(bool b) { _toRemove = b; }

	virtual void setPosition(float x, float y);

protected:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
	sf::CircleShape _shape;

	Map* _map; // Map this object is located on

	std::vector<Perk*> _perks;

	Stats _stats;

	Target* _target; // Target the enemy is running to (can be coord or enemy)
	Vector2 _direction; // Direction they're moving to

	bool _toRemove; // Is this object marked for removal?
	int  _collisionRadius;
};

#endif