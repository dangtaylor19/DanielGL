#include <Mesh.h>
#include <iostream>

void Mesh::ProcessMesh(aiMesh* mesh)
{	
	//Clear current model
	vertices.clear();
	indices.clear();
	
	//process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++){
		//Positions
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		//Texture coords
		if (mesh->mTextureCoords[0]) {
			vertices.push_back(1.0f - mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}

		//Normals
		vertices.push_back(mesh->mNormals[i].x);
		vertices.push_back(mesh->mNormals[i].y);
		vertices.push_back(mesh->mNormals[i].z);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		
	}
}
