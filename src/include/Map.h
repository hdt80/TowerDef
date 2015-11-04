#ifndef _MAP_H
#define _MAP_H

#include "Path.h"
#include "Tower.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Color.h"

#include <vector>

#define WAVE_DELAY 30 // Time between each wave in seconds
#define MAP_HEALTH 10 // Enemies that can reach the end before you lose

class Map {
public:
	Map();
	~Map();

	void update(int diff);
	void spawnWave();
	void spawnTower(float x, float y);
	void shoot(Tower* shooter, Enemy* e, Color c);

	Path* getPath() { return &_enemyPath; }
	int getHealth() { return _health; }
	int getMaxHealth() { return _maxHealth; }

	void setHealthRelative(int i) { _health += i; }

	Tower* towerAt(float x, float y);

	std::vector<Enemy*> enemies;
	std::vector<Tower*> towers;
	std::vector<Projectile*> projectiles; 

	std::vector<Object*> toRemove;

protected:
	Path _enemyPath; // Path enemies follow

	int _wave;		// Current wave number
	float _waveTime;// Time passed since last wave
	int _waveDelay;	// Time between each wave

	int _health;
	int _maxHealth;
};

#endif