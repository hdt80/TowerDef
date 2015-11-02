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
	for (unsigned int i = 0; i < enemies.size(); ++i) {
		delete enemies[i];
	}
	for (unsigned int i = 0; i < towers.size(); ++i) {
		delete towers[i];
	}
	enemies.clear();
	towers.clear();
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

	// Update enemies first as everything depends on their position
	for (unsigned int i = 0; i < enemies.size(); ++i) {
		enemies[i]->update(diff);
		// Don't delete them yet or towers and projectiles will seg fault if 
		// they are deleted. Simply mark them for deletion so towers and
		// projectiles will have a target to move to, however we still want
		// to remove them from the enemies vector or we'll still have a 
		// pointer to a removed enemy
		if (enemies[i]->isToRemove()) {
			toRemove.push_back(enemies[i]);
			enemies.erase(enemies.begin() + i);
		}
	}

	// Update towers second as their targetting relies on enemies, such as
	// if an enemy is going to be removed, because if it is stop shooting
	// at it and find a new target
	for (unsigned int i = 0; i < towers.size(); ++i) {
		towers[i]->update(diff);
	}

	// Now that all the updating is done we can safely remove all objects
	// that are marked for removal
	for (unsigned int i = 0; i < toRemove.size(); ++i) {
		delete toRemove[i];
	}
	toRemove.clear();

}

void Map::spawnWave() { 
	CORE_INFO("Spawning wave %i", ++_wave);
	enemies.push_back(new Enemy(this, 10.0f, 50, &_enemyPath, 20));
}

void Map::spawnTower(float x, float y) {
	towers.push_back(new Tower(this, x, y));
}