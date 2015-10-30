#include "Map.h"
#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor and deconstructor
///////////////////////////////////////////////////////////////////////////////
Map::Map() : wave(0), waveTime(0), waveDelay(WAVE_DELAY) {
	enemyPath.addPoint(0, 50);
	enemyPath.addPoint(50, 50);
	enemyPath.addPoint(50, 400);
	enemyPath.addPoint(400, 400);

	enemies.clear();
	towers.clear();
}

Map::~Map() {

}

///////////////////////////////////////////////////////////////////////////////
// Methods
///////////////////////////////////////////////////////////////////////////////

// The diff is provided in milliseconds
void Map::update(int diff) {
	CORE_INFO("Diff: %fs | waveTime: %fs", diff * 0.000001f, waveTime);
	if (waveTime >= waveDelay) {
		spawnWave();
		waveTime = 0;
	}
	waveTime += diff * 0.000001f;

	for (unsigned int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->isToRemove()) {
			CORE_INFO("Enemy %i is done", i);
			enemies.erase(enemies.begin() + i);
		}
		enemies[i]->update(diff);
	}
}

void Map::spawnWave() { 
	CORE_INFO("Spawning wave %i", ++wave);
	enemies.push_back(new Enemy(this, 10, 10, &enemyPath, 20));
}

void Map::spawnTower(float x, float y) {
	
}