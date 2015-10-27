#include "Map.h"
#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor and deconstructor
///////////////////////////////////////////////////////////////////////////////
Map::Map() : wave(0), waveTime(0), waveDelay(WAVE_DELAY) {
	enemyPath.addPoint(0, 25);
	enemyPath.addPoint(100, 25);
	enemyPath.addPoint(100, 100);
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
}

void Map::spawnWave() { 
	CORE_INFO("Spawning wave");
}