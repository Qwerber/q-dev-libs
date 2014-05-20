#pragma once
#include "glbatch.h"

namespace kc8
{
	struct Vector2D
	{
		float x;
		float y;
	};

	inline void vector2d_addTo(Vector2D* _a, const Vector2D _b)
	{
		_a->x += _b.x;
		_a->y += _b.y;
	}

	struct Particle2D
	{
		Vector2D position;
		Vector2D velocity;
		Vector2D acceleration;
	};

	inline void particle2d_update(Particle2D* _particle)
	{
		vector2d_addTo(&(_particle->velocity), (_particle->acceleration));
		vector2d_addTo(&(_particle->position), (_particle->velocity));
	}

	struct Emitter2D
	{
		Particle2D* particles;
		
		int numParticles;
		int insertionPoint;
		int maxParticles;

		// dynamic array
		Vector2D* globalForces;

		// filters feature

		//glbatch
		qsb::Batch* particleBatch;
	};

	Emitter2D* createEmitter(int _maxParticles);
	void emitParticles(Emitter2D* _emitter, int _numParticles, Vector2D _position, Vector2D _velocity);
	void pushParticle(Emitter2D* _emitter, Vector2D _position, Vector2D _velocity);
	void updateEmitter(Emitter2D* _emitter);
}