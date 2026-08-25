#include <string>
#include <Vector3.h>
#include <Sprite.h>
#include <EngineHeaders.h>
#include <shader.h>
#include <cstring>

#include <DrawableObject.h>

using namespace std;

class GameObject : public DrawableObject {
public:
	GameObject() {
		/*Vertex data*/
		float v[] = {
			//pos               //color            //texture coords
			 0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.9f,  1.0f, 1.0f,  // top right
			 0.5f, -0.5f, 0.0f,  1.0f, 0.9f, 0.0f,  1.0f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  0.5f, 0.1f, 0.6f,  0.0f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // top left 
		};
		unsigned int i[] = {
			0, 1, 3,  // first tri
			1, 2, 3   // second tri
		};

		std::memcpy(this->vertices, v, sizeof(v));

		std::memcpy(this->indices, i, sizeof(i));

		this->texturePath = "Assets/characters.png";

		/*Object properties*/
		this->sprite = Sprite(this->texturePath, 16, 24);

		this->position = Vector3(0, 0, 0);
		this->rotation = Vector3(0, 0, 0);
		this->scale = Vector3(1, 1, 1);

		this->transform = glm::mat4(1.0f);

		this->shader = Shader("engine/core/DefaultVertexShader.vs", "engine/core/DefaultFragmentShader.fs");
	}

	void InitializeShape();
	void DrawShape() ;
	void Draw(Camera& cam) override;
	void ClearBuffers() override;

	void SetName(const string& name);
	string GetName() const;

	void SetPos(const Vector3& pos);
	Vector3 GetPos() const;

	void SetRotation(const Vector3& rotation);
	Vector3 GetRotation() const;

	void SetScale(const Vector3& scale);
	Vector3 GetScale() const;

	glm::mat4 GetTransform() const;

	void SetSpriteIndex(const int index);

	void SetAnimated(bool animated);

private:
	/*Vertex parameters*/
	float vertices[32];
	unsigned int indices[6];

	unsigned int VAO, VBO, EBO;
	unsigned int texture;

	/*Basic parameters*/
	string name;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Sprite sprite;

	const char* texturePath;

	int spriteIndex;

	glm::mat4 transform;

	bool animatedSprite;

};