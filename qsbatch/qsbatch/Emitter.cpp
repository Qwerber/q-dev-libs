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

		ret->particleBatch = qsb::createBatch(2, _maxParticles);
		/*qsb::batch_setProgram(
			ret->particleBatch,
			qsb::createProgram(
			qsb::createShader(
			GL_VERTEX_SHADER,
			"#version 130\n"
			"in vec2 position;"
			"in vec2 corner;"
			"in vec4 color;"
			"out vec4 color_v;"
			"uniform mat4 gl_ModelViewMatrix;"
			"void main(){"
			"  gl_Position = gl_ModelViewMatrix * vec4(position.x + corner.x, position.y + corner.y,"
			"    0, 1);"
			"  color_v = color;"
			"}"
			),
			qsb::createShader(
			GL_FRAGMENT_SHADER,
			"#version 130\n"
			"in vec4 color_v;"
			"out vec4 LFragColor;"
			"void main(){"
			"  LFragColor = color_v;"
			"}"
			)
			)
		);

		qsb::batch_generateAttributeData(ret->particleBatch, "position{ff}corner{ff}color{ffff}");*/

		qsb::batch_setProgram(ret->particleBatch,
							  qsb::createProgram(
							  qsb::createShader(
							  GL_VERTEX_SHADER,
							  "#version 130\n"
							  "in vec2 LVertexPos2D;"
							  "out vec4 color;"
							  "uniform mat4 gl_ModelViewMatrix;"
							  "void main() {"
							  "  color = vec4(1,(LVertexPos2D.x-200)/111,(LVertexPos2D.x - 200)/111,1);"
							  "  gl_Position = gl_ModelViewMatrix * vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); "
							  "}"
							  ),
							  qsb::createShader(
							  GL_FRAGMENT_SHADER,
							  "#version 130\n in vec4 color; out vec4 LFragment;  void main() { LFragment  = color; }"
							  )));

		qsb::batch_generateAttributeData(ret->particleBatch, "LVertexPos2D{ff}");

		qsb::batch_PrintAttributeData(ret->particleBatch);

		return ret;
	}

	void renderEmitter(Emitter2D* _emitter)
	{
		qsb::Batch * b = _emitter->particleBatch;
		int ind = 0;
		int i = _emitter->numParticles;
		while (i--)
		{
			printf("ind %d", ind);

			qsb::batch_pushVertex(b, _emitter->particles[i].position.x, _emitter->particles[i].position.y);
			qsb::batch_pushIndex(b, ind++);
			qsb::batch_pushVertex(b, _emitter->particles[i].position.x, _emitter->particles[i].position.y + 111);
			qsb::batch_pushIndex(b, ind++);
			qsb::batch_pushVertex(b, _emitter->particles[i].position.x + 111, _emitter->particles[i].position.y);
			qsb::batch_pushIndex(b, ind++);
			qsb::batch_pushVertex(b, _emitter->particles[i].position.x + 111, _emitter->particles[i].position.y);
			qsb::batch_pushIndex(b, ind++);
			qsb::batch_pushVertex(b, _emitter->particles[i].position.x + 111, _emitter->particles[i].position.y + 111);
			qsb::batch_pushIndex(b, ind++);
			qsb::batch_pushVertex(b, _emitter->particles[i].position.x, _emitter->particles[i].position.y + 111);
			qsb::batch_pushIndex(b, ind++);

			printf("%f", _emitter->particles[i].position.x);
		}

		qsb::drawBatch(b);
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