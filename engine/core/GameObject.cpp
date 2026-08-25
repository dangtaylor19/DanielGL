#include <DanielHeaders.h>
#include <EngineHeaders.h>
#include <stb_image.h>
#include <iostream>
#include <string>

using namespace std;

void GameObject::InitializeShape(){
	/*Load Texture*/
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, 16, 24, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, animatedSprite ? this->sprite.GetAnimatedUV(spriteIndex + 1, 2) : this->sprite.GetUVData(spriteIndex));
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	/*Vertex*/

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*Link vertex attributes*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/*Link color attribute*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	/*Link texture attributes*/
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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

void GameObject::DrawShape() {
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GameObject::Draw(Camera& cam)  {
	shader.use();
	shader.setMat4("transform", GetTransform());
	DrawShape();
}

void GameObject::ClearBuffers() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void GameObject::SetName(const string& name) {this->name = name;}
string GameObject::GetName() const{return name;}

void GameObject::SetPos(const Vector3& pos) {
	this->transform = glm::translate(this->transform, glm::vec3(pos.x-this->position.x, pos.y-this->position.y, pos.z-this->position.z));
	this->position = pos;
}
Vector3 GameObject::GetPos() const { return this->position; }

void GameObject::SetRotation(const Vector3& rotation) {
	this->transform = glm::rotate(this->transform, glm::radians(this->rotation.x - rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));//x
	this->transform = glm::rotate(this->transform, glm::radians(this->rotation.y - rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));//y
	this->transform = glm::rotate(this->transform, glm::radians(this->rotation.z - rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));//z
	this->rotation = rotation;
}
Vector3 GameObject::GetRotation() const { return this->rotation; }

void GameObject::SetScale(const Vector3& scale) {
	this->transform = glm::scale(this->transform, glm::vec3(scale.x / this->scale.x, scale.y / this->scale.y, scale.z / this->scale.z));
	this->scale = scale;
}
Vector3 GameObject::GetScale() const { return this->scale; }

glm::mat4 GameObject::GetTransform() const {
	return this->transform;
}

void GameObject::SetSpriteIndex(const int index) {
	this->spriteIndex = index;
}

void GameObject::SetAnimated(bool animated) {
	this->animatedSprite = animated;
}