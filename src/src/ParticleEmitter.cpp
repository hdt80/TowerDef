#include "ParticleEmitter.h"

#include "Window.h"
#include "Particle.h"

Window* ParticleEmit::window; // Create a reference to the window

namespace ParticleEmit {
	void emit(float x, float y, int amount, int r, int g, int b) {
		ParticleEmitter* pe = new ParticleEmitter(sf::Vector2f(x, y),
			amount, sf::Color(r, g, b));
		window->emitters.push_back(pe);
	}
}