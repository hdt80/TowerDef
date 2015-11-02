#include "Map.h"
#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////
// Constructor and deconstructor
///////////////////////////////////////////////////////////////////////////////
Map::Map() : _wave(0), _waveTime(0), _waveDelay(WAVE_DELAY),
	_health(MAP_HEALTH), _maxHealth(_health) {

	_enemyPath.addPoint(0, 50);
	_enemyPath.addPoint(50, 50);
	_enemyPath.addPoint(50, 400);
	_enemyPath.addPoint(400, 400);

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
	//CORE_INFO("Diff: %fs | waveTime: %fs", diff * 0.000001f, _waveTime);
	if (_waveTime >= _waveDelay) {
		spawnWave();
		_waveTime = 0;
	}
	_waveTime += diff * 0.000001f;

	for (unsigned int i = 0; i < enemies.size(); ++i) {
		enemies[i]->update(diff);
		if (enemies[i]->isToRemove()) {
			Enemy* e = enemies[i];
			enemies.erase(enemies.begin() + i);
			//CORE_INFO("Enemy %i is done", i);
			delete e;
		}
	}

	for (unsigned int i = 0; i < towers.size(); ++i) {
		towers[i]->update(diff);
	}
}

void Map::spawnWave() { 
	CORE_INFO("Spawning wave %i", ++_wave);
	enemies.push_back(new Enemy(this, 10.0f, 50, &_enemyPath, 20));
}

void Map::spawnTower(float x, float y) {
	towers.push_back(new Tower(this, x, y));
}