#ifndef _MAP_H
#define _MAP_H

#include "Path.h"
#include "Tower.h"
#include "Enemy.h"

#include <vector>

#define WAVE_DELAY 30 // Time between each wave in seconds

class Map {
public:
	Map();
	~Map();

	void update(int diff);
	void spawnWave();

	Path* getPath() { return &enemyPath; }

	std::vector<Enemy*> enemies;
	std::vector<Tower*> towers;

protected:
	Path enemyPath;

	int wave;		// Current wave
	float waveTime;	// Time passed since last wave
	int waveDelay;	// Time between each wave
};

#endif