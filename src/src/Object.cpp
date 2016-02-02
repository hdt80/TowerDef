#include "Object.h"

#include "Perk.h"
#include <string>
#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Object::Object(Map* map, float x, float y, int collRadius, Stats s) :
	Target(x, y), _map(map), _tree(nullptr),  _attackerCount(0), _baseStats(s),
	_target(nullptr), _toRemove(false), _collisionRadius(collRadius) {

}

Object::Object() :
	Target(0, 0), _map(nullptr), _stats(Stats()), _target(nullptr),
	_toRemove(false), _collisionRadius(0) {
}

Object::~Object() {}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const {
}

void Object::onCollision(Object* o) {
	CORE_INFO("%i collided with %i", this, o);
}

bool Object::collidesWith(Object* o) {
	return (getCollisionBox().intersects(o->getCollisionBox()));
}

// Returns if a point is within the collision box of an Object
bool Object::contains(float px, float py) {
	return (px >= getX() - _collisionRadius &&
			px <= getX() + _collisionRadius &&
			py >= getY() - _collisionRadius &&
			py <= getY() + _collisionRadius);
}

void Object::move(int diff) {
	// No need to calculate movement if they can't move
	if (getSpeed() <= 0) {
		return;
	}
	// If we don't have anywhere we've moving to don't move
	if (_target == nullptr) {
		_direction = Vector2();
		return;
	}

	Vector2 to(_target->getX(), _target->getY());
	Vector2 cur(x, y);

	Vector2 goingTo(to - cur);
	_direction = goingTo.normalize();

	double deltaMove = (double)getSpeed() * 0.000001f * diff;

	x += _direction.X * deltaMove;
	y += _direction.Y * deltaMove;
}

void Object::update(int diff) {
	for (unsigned int i = 0; i < _perks.size(); ++i) {
		_perks[i]->update(diff);
		if (_perks[i]->isToRemove()) {
			removePerk(_perks[i]);
			_perks.erase(_perks.begin() + i);
		}
	}
}

void Object::setStats(Stats s, bool relative) {
	if (relative) {
		_baseStats.range     += s.range;
		_baseStats.fireRate  += s.fireRate;
		_baseStats.damage    += s.damage;
		_baseStats.speed     += s.speed;
        _baseStats.projSpeed += s.projSpeed;
        _baseStats.accel     += s.accel;
	} else {
		_baseStats = s;
	}
}

void Object::applyStat(Stats s) {
    if (!s.percent) {
        CORE_WARNING("Object::applyStat> Stats isn't percent");
    }
	CORE_INFO("!!! Stats before !!!");
	_stats.print();
    _stats.range += _baseStats.range * s.range;
    _stats.fireRate += _baseStats.fireRate * s.fireRate;
    _stats.damage += _baseStats.damage * s.damage;
    _stats.projSpeed += _baseStats.projSpeed * s.projSpeed;
    _stats.speed += _baseStats.speed * s.speed;
    _stats.accel += _baseStats.accel * s.accel;
	CORE_INFO("!!! Stats after !!!");
	_stats.print();
}

void Object::removePerk(Perk* p) {
	applyStat(-*p->getStats());
}

void Object::addPerk(Perk* p) {
	// If we already have the buff
	if (getPerk(p->getName()) != nullptr) {
		Perk* curP = getPerk(p->getName());
		// Stackable and we can add a stack? Apply stat change and add 1 stack
		if (p->isStackable() && (curP->getStacks() < curP->getMaxStacks())) {
			// Add the stat mod to the current perk so removal is correct
			*curP->getStats() += *p->getStats();
			curP->addStack();
			applyStat(*p->getStats());
		}
		// Reset duration
		curP->setDuration(p->getDuration());
	} else {
		_perks.push_back(p);
		applyStat(*p->getStats());
	}
}

Perk* Object::getPerk(std::string name) {
	for (unsigned int i = 0; i < _perks.size(); ++i) {
		if (_perks[i]->getName() == name) {
			return _perks[i];
		}
	}
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// Getters and setters
///////////////////////////////////////////////////////////////////////////////
void Object::setTarget(Target* t) {
	// Are we already targetting the target?
	if (_target == t) {
		return;
	}

	// If we aren't targetting the same target we check if we need to delete
	// the old target. If it's a simple target we can delete it safely, but if
	// it isn't we will be deleting an object that is in use
	if (_target && _target->isSimpleTarget()) {
		delete _target;
	}
	_target = t;
}

void Object::setPosition(float nx, float ny) {
	x = nx;
	y = ny;
	setTarget(nullptr);
}

void Object::setSkillTree(SkillTree* tree) {
	_tree = tree->clone();
	_tree->setAttached(this);
}
