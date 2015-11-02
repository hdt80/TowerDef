#ifndef _MAP_H
#define _MAP_H

#include "Path.h"
#include "Tower.h"
#include "Enemy.h"

#include <vector>

#define WAVE_DELAY 30 // Time between each wave in seconds
#define MAP_HEALTH 10 // Amount of enemies that can pass before you lsoe

class Map {
public:
	Map();
	~Map();

	void update(int diff);
	void spawnWave();
	void spawnTower(float x, float y);

	Path* getPath() { return &_enemyPath; }
	int getHealth() { return _health; }
	int getMaxHealth() { return _maxHealth; }

	void setHealthRelative(int i) { _health += i; }

	std::vector<Enemy*> enemies;
	std::vector<Tower*> towers;

	std::vector<Object*> toRemove;

protected:
	Path _enemyPath; // Path enemies follow

	int _wave;		// Current wave
	float _waveTime;// Time passed since last wave
	int _waveDelay;	// Time between each wave

	int _health;
	int _maxHealth;
};

#endif