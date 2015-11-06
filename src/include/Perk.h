#ifndef _PERK_H
#define _PERK_H

#include <string>
#include "Stats.h"

class Perk {
public:
	Perk(std::string name, Stats s, float dur, bool stacks = false);
	~Perk();

	void update(int diff);

	std::string getName() const { return _name; }
	Stats* getStats() { return &_stats; }
	float getDuration() const { return _duration; }
	int getStacks() const { return _stacks; }
	bool isStackable() const { return _stackable; }
	bool isToRemove() const { return _toRemove; }

	void setDuration(float d) { _duration = d; }
	void addStack() { _stacks++; }
	void removeStack() { _stacks--; }

protected:
	std::string _name; // Display name of the perk
	Stats _stats; // Stats applied to the Object
	float _duration; // -1 duration means infinite

	int _stacks;
	// If statckable applying a new perk with the same name will add a stack
	// If not stackable and applying the same perk the _duration will be reset
	// to the new perks duration
	bool _stackable; 

	bool _toRemove; // Marked for removal

	
};

#endif