#include <Mesh.h>

class Light;
class Module;

class Renderer3D : public Module{
public:
	Renderer3D(Sprite* sprite){
		this->name = "Renderer";

		this->mesh = std::make_shared<Mesh>();

		SetSprite(sprite);

		this->shader = std::make_shared<Shader>("engine/core/DefaultVertexShader.vs", "engine/core/DefaultFragmentShader.fs");
	}

	void OnPlay() override;
	void OnUpdate() override;

	void DrawInspector() override;

	virtual void Serialize(ofstream& ofstream) const override;
	//virtual void Deserialize() override;

	void InitializeShape();
	void DrawShape() ;
	void Draw(Camera& cam, std::vector<std::shared_ptr<Light>>& lights);
	void ClearBuffers();

	void SetSprite(Sprite* s);

	void SetMesh(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<Mesh> mesh;

private:
	unsigned int VAO, VBO, EBO;
	unsigned int texture;

	/*Basic parameters*/
	Sprite* sprite;

	std::shared_ptr<Shader> shader;

	const char* texturePath = "Assets/wall.jpg";;

	int spriteIndex;

	bool animatedSprite;
};