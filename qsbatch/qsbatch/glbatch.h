#pragma once
	
#include <SDL.h>
#include <SDL_opengl.h>

namespace qsb
{
	extern const int OK;
	extern const int FAIL;

#pragma region "modern"
	////////////////////
	// MODERN GLBATCH

	/*
	 * Data structure for storing information about vertex attributes 
	 * This helps the batch to properly link with OpenGL
	 */
	struct VertexAttribute
	{
		GLint 
			// Location of the vertex attribute
			location,
			// One of 1,2,3 or 4, the amoun of componenets the "vec" has
			dim,
			// Starting byte offset of the attribute used for offset
			start,
			// The "stride" in bytes
			skip;

		// The value type
		GLenum type;
	};

	struct GLBatch
	{
		/*
		 * Create a new GLBatch Object 
		 * @param _bytesPerVertex  The number of bytes to allocate for each vertex
		 * @param _vertextLength   The maximum number of vertexes that the structure can hold
		 * @param _indexLength     The maximum amount of elements in the indexBuffer
		 */
		GLBatch(GLuint _bytesPerVertex, GLuint _vertexLength, GLuint _indexLength);

		//destructor
		~GLBatch();

		/*
		 * Resets the GLBatch
		 * This function resets dataPointer to original locations and sets writingIndex to 0
		 */
		void reset();

		/*
		 * Sets the program that the GLBatch will use when draw() is called
		 * @param _program  The program that the GLBatch will use
		 */
		void setProgram(GLuint _program);

		/*
		 * Gives the GLBatch information that it will use when configuring vbos and buffering data
		 * The format is as follows:
		 *  <attribute><rest> where:
		 *   <attribute> is:
		 *    <field name>{<bytes per data><char corresponding to type x (data amount)>}
		 *   <rest> is:
		 *    <attribute><or><nothing>
		 *
		 * Data is assumed to be aligned in same order 
		 *
		 * Example data: position{4ff}color{1bbb}uv{4ff}
		 */
		void generateAttributeData(char* _data);

		/*
		 * Outputs information about the shaderprogram and its attribute
		 */
		void printAttributeData();

		template<typename DataType>
		inline void* pushData(DataType _data)
		{
			void* ret = vbDataPointer;
			*((DataType*)vbDataPointer) = _data;
			vbDataPointer = (DataType*)vbDataPointer + 1;
			return ret;
		}

		inline void pushIndex(GLuint _i) { *(ibDataPointer++) = _i; }

		inline void pushQuadIndex()
		{
			pushIndex(writingIndex++);
			pushIndex(writingIndex++);
			pushIndex(writingIndex);
			pushIndex(--writingIndex);
			pushIndex(++writingIndex);
			pushIndex(++writingIndex);
			writingIndex++;
		}

		GLuint
			bytesPerVertex,
			vertexLength,
			indexLength,
			writingIndex,
			
			*ibDataPointer,
			*indexData,

			shaderProgram,
			
			numAttributes;

		void* vertexData;
		void* vbDataPointer;

		VertexAttribute attributes[16];		
	};

	struct Batch
	{
	
		GLuint dataPerVertex;
		GLuint length;

		GLuint shaderProgram;

		GLfloat* vertextData;
		GLuint* indexData;

		GLfloat* vbDataPointer;
		GLuint* ibDataPointer;
		GLuint vertexCount;

		VertexAttribute attributes[16];
		int numAttributes;
		
		GLuint indexValue;
	};
#pragma endregion

	Batch* createBatch(int _dataPerVertex, int _length);
	void destroyBatch(Batch* _batch);
	void batch_reset(Batch* _batch);
	void batch_setProgram(Batch* _batch, GLuint _program);
	void batch_generateAttributeData(Batch* _batch, char* _data);
	void batch_PrintAttributeData(Batch* _batch);

#pragma region "push functions"

	inline void batch_pushIndex(Batch* _batch, GLuint _i)
	{
		//_batch->indexData[_batch->ibDataPointer ++] = _i;
		*(_batch->ibDataPointer++) = _i;
	}

	inline void batch_pushVertData(Batch* _batch, GLfloat _x)
	{
		//_batch->vertextData[_batch->vbDataPointer ++] = _x;
		*(_batch->vbDataPointer++) = _x;
	}

	inline void batch_pushQuadIndex(Batch* _batch)
	{
		batch_pushIndex(_batch, _batch->indexValue++);
		batch_pushIndex(_batch, _batch->indexValue++);
		batch_pushIndex(_batch, _batch->indexValue);
		batch_pushIndex(_batch, --_batch->indexValue);
		batch_pushIndex(_batch, ++_batch->indexValue);
		batch_pushIndex(_batch, ++_batch->indexValue);
		_batch->indexValue++;
	}

#pragma endregion

	int initGLBatch(int _screenWidth, int _screenHeight, int _defaultColor, SDL_Window* _window);

	GLuint createShader(GLuint _shaderType, const GLchar* _shaderCode);
	GLuint createProgram(GLuint _vertexShader, GLuint _fragmentShader);

	// Once you set the screen dimensions once through initialization you cannot change them again
	int getScreenWidth();
	int getScreenHeight();

	int pushBatch(Batch* _batch);
	int drawBatch(Batch* _b);
	int drawGLBatch(GLBatch* _b);
	int drawAllBatches();
	int display();

	int cleanup();

	// debug functions


}