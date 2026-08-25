#include <DanielHeaders.h>
#include <EngineHeaders.h>
#include <stb_image.h>
#include <iostream>
#include <string>

using namespace std;

void Renderer3D::OnPlay() {
	
}

void Renderer3D::OnUpdate() {
	
}

void Renderer3D::DrawInspector(){

}

void Renderer3D::Serialize(ofstream& ofstream) const{
	
}

void Renderer3D::InitializeShape() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, this->sprite->GetChannels() == 3 ? GL_RGB : GL_RGBA, this->sprite->GetWidth(), this->sprite->GetHeight(), 0, this->sprite->GetChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, this->sprite->GetUVData(0));
	glGenerateMipmap(GL_TEXTURE_2D);

	/*Vertex*/

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(float), mesh.get()->vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), mesh.get()->indices.data(), GL_STATIC_DRAW);

	/*Link vertex attributes*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/*Link texture attributes*/
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	/*Link normal attributes*/
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/*Alignment*/
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*Wrapping*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	/*Filtering*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/*Mipmaps*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Renderer3D::DrawShape() {
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer3D::Draw(Camera& cam, std::vector<std::shared_ptr<Light>>& lights) {
	shader.get()->use();

	//Ensure owner is not nullptr
	if (auto ownerPtr = owner.lock()) {
		shader.get()->setMat4("transform", ownerPtr->GetTransform());
		shader.get()->setMat4("projection", cam.GetProjection());
		shader.get()->setMat4("view", cam.GetView());

		shader.get()->setVec3("lightPos", 1, 0, 0);
		shader.get()->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader.get()->setVec3("viewPos", cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);

		shader.get()->setFloat("lightCount", lights.size());
	}

	for (size_t i = 0; i < lights.size(); i++)
	{
		//Ensure owner is not nullptr
		if (auto ownerPtr = lights[i]->owner.lock()) {
			Vector3 pos = ownerPtr->GetPos();
			Vector3 col = lights[i].get()->color;
			shader.get()->setVec3("pointLights[" + std::to_string(i) + "].position", pos.x, pos.y, pos.z);
			shader.get()->setFloat("pointLights[" + std::to_string(i) + "].range", lights[i].get()->radius);
			shader.get()->setFloat("pointLights[" + std::to_string(i) + "].intensity", lights[i].get()->intensity);
			shader.get()->setVec3("pointLights[" + std::to_string(i) + "].color", col.x, col.y, col.z);
		}
	}

	DrawShape();
}

void Renderer3D::ClearBuffers() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Renderer3D::SetSprite(Sprite* newSprite) {
	this->sprite = newSprite;
	this->texturePath = newSprite->GetPath();

	InitializeShape();
}

void Renderer3D::SetMesh(std::shared_ptr<Mesh> mesh){
	this->mesh = mesh;
}
