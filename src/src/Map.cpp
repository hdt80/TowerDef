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

	for (unsigned int i = 0; i < projectiles.size(); ++i) {
		projectiles[i]->update(diff);
		if (projectiles[i]->isToRemove() ||
				// Projectile outside map?
				(projectiles[i]->getX() < 0 ||
				projectiles[i]->getX() > _width ||
				projectiles[i]->getY() < 0 ||
				projectiles[i]->getY() > _height)) {

			toRemove.push_back(projectiles[i]);
			projectiles.erase(projectiles.begin() + i);
		}
	}

	// Now that all the updating is done we can safely remove all objects
	// that are marked for removal
	for (unsigned int i = 0; i < toRemove.size(); ++i) {
		delete toRemove[i];
	}
	toRemove.clear();
	
	calcCollisions();
}

void Map::calcCollisions() {
	for (unsigned int i = 0; i < projectiles.size(); ++i) {
		for (unsigned int j = 0; j < enemies.size(); ++j) {
			if (projectiles[i]->collidesWith(enemies[j])) {
				projectiles[i]->onCollision(enemies[j]);
			}
		}
	}
}

// Spawn a new wave
void Map::spawnWave() { 
	CORE_INFO("Spawning wave %i", ++_wave);
	Stats enemyStats;
	enemyStats.speed = 50;
	enemies.push_back(new Enemy(this, 10.0f, enemyStats, &_enemyPath, 20));
}

// Spawn a tower at (x, y)
void Map::spawnTower(float x, float y) {
	CORE_INFO("Spawning tower at (%f, %f)", x, y);
	Stats towerStats;
	towerStats.range = 200;
	towerStats.fireRate = 1.0f;
	towerStats.damage = 1.0f;
	towerStats.speed = 0.0f;
	towerStats.projSpeed = 200.0f; 
	towers.push_back(new Tower(this, x, y, towerStats));
}

// Return the Object at (x, y)
// If no Object is there return nullptr 
Tower* Map::towerAt(float x, float y) {
	for (unsigned int i = 0; i < towers.size(); ++i) {
		// Point within region that tower is in
		if (towers[i]->contains(x, y)) {
			return towers[i];
		}
	}
	return nullptr;
}

// Tower t is shooting at e, so spawn a projectile and begin tracking it
void Map::shoot(Tower* t, Projectile* p) {
	projectiles.push_back(p);
	//projectiles.push_back(new Projectile(this, e, t, c));
}