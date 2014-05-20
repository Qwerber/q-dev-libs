#include "stdafx.h"
#include "Emitter.h"
#include <stdlib.h>
#include "glbatch.h"

namespace kc8
{

	Emitter2D* createEmitter(int _maxParticles)
	{
		Emitter2D* ret = (Emitter2D*)malloc(sizeof(Emitter2D));
		ret->numParticles = 0;
		ret->insertionPoint = 0;
		ret->maxParticles = _maxParticles;

		ret->particles = (Particle2D*)malloc(_maxParticles * sizeof(Particle2D));
		ret->particleBatch = qsb::createBatch(8, _maxParticles);
		qsb::batch_setProgram(
			qsb::createProgram(
				qsb::createShader(
				GL_VERTEX_SHADER,
				"#version 130\n"
				"in vec2 position"
				"in vec2 corner"
				"in vec4 color"
				"uniform mat4 gl_ModelV"
				)));
	}

	void emitParticles(Emitter2D* _emitter, int _numParticles, Vector2D _position, Vector2D _velocity)
	{
		{
			int i = 0;
			while (i < _numParticles)
			{
				pushParticle(_emitter, _position, _velocity);
				i++;
			}
		}
	}

	void pushParticle(Emitter2D* _emitter, Vector2D _position, Vector2D _velocity)
	{	
		int ins = _emitter->insertionPoint;

		if (ins == _emitter->maxParticles)
			_emitter->insertionPoint = ins = 0;

		//fix this later
		_emitter->particles[ins].acceleration = { 0, 0.2 };
		_emitter->particles[ins].position = _position;
		_emitter->particles[ins].velocity = _velocity;

		_emitter->insertionPoint++;
		_emitter->numParticles++;
	}

	void updateEmitter(Emitter2D* _emitter)
	{
		int i = _emitter->numParticles;
		while (i--)
		{
			particle2d_update(&(_emitter->particles[i]));
		}
	}

}