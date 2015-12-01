#include "Perk.h"

#include "Logger.h"
#include "Convert.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Perk::Perk(std::string name, Stats s, float dur, int maxStacks) :
	_name(name), _stats(s), _duration(dur), _maxDuration(dur), _stacks(1),
	_maxStacks(maxStacks), _stackable(false), _toRemove(false) {

	if (maxStacks > 0) {
		_stackable = true;
	}
}

Perk::~Perk() {
	delete &_stats;
}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
std::string Perk::getTitle() const {
	return (getName() + "(" + convert::toString(getStacks()) + "/" +
		convert::toString(getMaxStacks()) + ")");
}

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

void Perk::addStack() {
	if (_stackable && (_stacks < _maxStacks)) {
		++_stacks;
	}
}