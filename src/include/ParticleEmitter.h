#ifndef _PARTICLE_EMITTER_H
#define _PARTICLE_EMITTER_H

#include <SFML/Graphics.hpp>

class Window;

namespace ParticleEmit {
	void emit(float x, float y, int amount, int r, int g, int b);

	extern Window* window;
};

#endif