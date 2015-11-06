#include "Perk.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Perk::Perk(std::string name, Stats s, float dur, bool stackable) :
	_name(name), _stats(s), _duration(dur), _stacks(0),
	_stackable(stackable), _toRemove(false) {

}

Perk::~Perk() {

}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////
void Perk::update(int diff) {

}