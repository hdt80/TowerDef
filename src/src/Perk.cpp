#include "Perk.h"

#include "Logger.h"
#include "Convert.h"
#include "Object.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Perk::Perk(std::string name, Stats s, float dur, int maxStacks) :
	_name(name), _stats(s), _duration(dur), _maxDuration(dur),
    _stacks(1),	_maxStacks(maxStacks), _stackable(false), _toRemove(false) {

	if (maxStacks > 0) {
		_stackable = true;
	}
}

Perk::~Perk() {
	delete &_stats;
}

Perk* Perk::clone() {
	if (this == nullptr) {
		return nullptr;
	}
	Perk* clone = new Perk(_name, _stats, _maxDuration, _maxStacks);
	clone->setStackable(_stackable);
	return clone;
}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Perk::update(int diff) {
	if (_duration <= -1.0f) {
		return;
	}
	_duration -= (diff * 0.000001f);
	if (_duration <= 0.0f) {
		CORE_INFO("\'%s\' is done", getName().c_str());
		_toRemove = true;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Getters and Setters
///////////////////////////////////////////////////////////////////////////////

std::string Perk::getTitle() const {
	return (getName() + "(" + convert::toString(getStacks()) + "/" +
		convert::toString(getMaxStacks()) + ")");
}

void Perk::setStacks(int c) {
    if (c > _maxStacks || c < 0) {
        CORE_INFO("Perk:: c: %i, _maxStacks: %i", c, _maxStacks);
        return;
    }
    if (!_stackable) {
        _duration = _maxDuration;
    } else {
        _stacks = c;
    }
}

void Perk::addStack() {
	if (_stackable && (_stacks < _maxStacks)) {
		++_stacks;
	} else {
        _duration = _maxDuration;
    }
}
