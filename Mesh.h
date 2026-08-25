
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Mesh {
public:
    Mesh() {
		/*Vertex data*/
		float v[] = {
			//pos             //texture coords   //normal
			//front
			0.5f,  0.5f,  0.5f,   1.0f, 1.0f,	 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 	 0.0f, 0.0f, 1.0f,
		   -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 	 0.0f, 0.0f, 1.0f,
		   -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 	 0.0f, 0.0f, 1.0f,

		   //back
		   0.5f,  0.5f, -0.5f,   0.0f, 1.0f,	 0.0f, 0.0f, -1.0f,
		  -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,	 0.0f, 0.0f, -1.0f,
		  -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,	 0.0f, 0.0f, -1.0f,
		   0.5f, -0.5f, -0.5f,   0.0f, 0.0f,	 0.0f, 0.0f, -1.0f,

		   //left
		   -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,	 -1.0f, 0.0f, 0.0f,
		   -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,	 -1.0f, 0.0f, 0.0f,
		   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,	 -1.0f, 0.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,	 -1.0f, 0.0f, 0.0f,

		   //right
		   0.5f,  0.5f, -0.5f,   1.0f, 1.0f,	 1.0f, 0.0f, 0.0f,
		   0.5f, -0.5f, -0.5f,   1.0f, 0.0f,	 1.0f, 0.0f, 0.0f,
		   0.5f, -0.5f,  0.5f,   0.0f, 0.0f,	 1.0f, 0.0f, 0.0f,
		   0.5f,  0.5f,  0.5f,   0.0f, 1.0f,	 1.0f, 0.0f, 0.0f,

		   //top
			0.5f,  0.5f, -0.5f,   1.0f, 0.0f,	 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,   1.0f, 1.0f,	 0.0f, 1.0f, 0.0f,
		   -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,	 0.0f, 1.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,	 0.0f, 1.0f, 0.0f,

		   //bottom
			0.5f, -0.5f,  0.5f,   1.0f, 1.0f,	 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, -0.5f,   1.0f, 0.0f,	 0.0f, -1.0f, 0.0f,
		   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,	 0.0f, -1.0f, 0.0f,
		   -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,	 0.0f, -1.0f, 0.0f,
		};
		unsigned int i[] = {
			0, 1, 3,  1, 2, 3,        // front
			4, 5, 7,  5, 6, 7,        // back
			8, 9, 11, 9, 10,11,       // left
			12,13,15, 13,14,15,       // right
			16,17,19, 17,18,19,       // top
			20,21,23, 21,22,23        // bottom
		};

		vertices.assign(v, v + sizeof(v) / sizeof(v[0]));

		indices.assign(i, i + sizeof(i) / sizeof(i[0]));
    }

    void ProcessMesh(aiMesh* mesh);

	/*Vertex parameters*/
	vector<float> vertices;
	vector<unsigned int> indices;
private:
    string directory;
};