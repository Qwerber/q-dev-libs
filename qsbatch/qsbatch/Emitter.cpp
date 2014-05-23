#include "stdafx.h"
#include "Emitter.h"
#include <stdlib.h>
#include "glbatch.h"
#include <Windows.h>

namespace kc8
{

	Emitter2D* createEmitter(int _maxParticles)
	{
		Emitter2D* ret = (Emitter2D*)malloc(sizeof(Emitter2D));
		ret->numParticles = 0;
		ret->insertionPoint = 0;
		ret->maxParticles = _maxParticles;

		ret->particles = (Particle2D*)malloc(_maxParticles * sizeof(Particle2D));

		ret->particleBatch = qsb::createBatch(5, _maxParticles * 6);

		qsb::batch_setProgram(ret->particleBatch,
							  qsb::createProgram(
							  qsb::createShader(
							  GL_VERTEX_SHADER,
							  "#version 130\n"
							  "in vec2 LVertexPos2D;"
							  "in vec3 color;"
							  "out vec3 color_v;"
							  "uniform mat4 gl_ModelViewMatrix;"
							  "void main() {"
							  "  color_v = color;"
							  "  gl_Position = gl_ModelViewMatrix * vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); "
							  "}"
							  ),
							  qsb::createShader(
							  GL_FRAGMENT_SHADER,
							  "#version 130\n"
							  "in vec3 color_v;"
							  "out vec4 LFragment;"
							  "void main() { "
							  "  LFragment  = vec4(color_v.x, color_v.y, color_v.z, 1); "
							  "}"
							  )));

		qsb::batch_generateAttributeData(ret->particleBatch, "LVertexPos2D{ff}color{fff}");

		qsb::batch_PrintAttributeData(ret->particleBatch);

		return ret;
	}

	void renderEmitter(Emitter2D* _emitter)
	{

		unsigned int t = GetTickCount();

		qsb::Batch* b = _emitter->particleBatch;
		int ind = 0;
		int i = _emitter->numParticles;
		while (i--)
		{

			Vector2D p = _emitter->particles[i].position;

			qsb::batch_pushVertData(b, p.x);
			qsb::batch_pushVertData(b, p.y);
			qsb::batch_pushVertData(b, 1);
			qsb::batch_pushVertData(b, 1);
			qsb::batch_pushVertData(b, 0);

			qsb::batch_pushVertData(b, p.x + 20);
			qsb::batch_pushVertData(b, p.y);
			qsb::batch_pushVertData(b, 1);
			qsb::batch_pushVertData(b, 1);
			qsb::batch_pushVertData(b, 0);

			qsb::batch_pushVertData(b, p.x);
			qsb::batch_pushVertData(b, p.y + 20);
			qsb::batch_pushVertData(b, 1);
			qsb::batch_pushVertData(b, 1);
			qsb::batch_pushVertData(b, 0);

			qsb::batch_pushVertData(b, p.x + 20);
			qsb::batch_pushVertData(b, p.y + 20);
			qsb::batch_pushVertData(b, 1);
			qsb::batch_pushVertData(b, 1);
			qsb::batch_pushVertData(b, 0);

			qsb::batch_pushQuadIndex(b);
		}
		printf("main: %u\n", GetTickCount() - t);
		qsb::drawBatch(b);
		qsb::batch_reset(b);

		
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
		_emitter->particles[ins].acceleration = { 0, 0.002 };
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