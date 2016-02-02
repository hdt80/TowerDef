#ifndef _EVENT_LISTENER_H
#define _EVENT_LISTENER_H

#include "Map.h"

class EventListener {
public:
    EventListener();
    ~EventListener();

    void onEnemyKill();
    void onDeath();
    void onShoot();
    void onDamageDealt();
    void onDamageTaken();
};

#endif
