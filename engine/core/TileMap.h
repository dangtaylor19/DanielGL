#include <EngineHeaders.h>

#include <DrawableObject.h>
#include <vector>


class TileMap : public DrawableObject{
public:
	TileMap(const int width, const int height) : width(10), height(10){
		this->width = width;
		this->height = height;

		this->tileMapData.assign(width * height, 1);

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

		this->tileMapSprite = Sprite("Assets/tiles1.png", 16);

		std::memcpy(this->vertices, v, sizeof(v));
		std::memcpy(this->indices, i, sizeof(i));

		this->transform = glm::mat4(1.0f);
		//this->transform = glm::translate(this->transform, glm::vec3(-width/2,height/2,0));

		this->layers.assign(2, layer(width, height, LoadMapData("Assets/tiles1.csv")));
		this->layers[1].layerData = LoadMapData("Assets/tiles2.csv");

		this->shader = Shader("engine/core/DefaultVertexShader.vs", "engine/core/DefaultFragmentShader.fs");
	}

	void InitializeTile(const char* texturePath, const int layerNum, const int tileNum);

	void DrawTile(const int tileNum, const int layerNum) const;

	void Draw(Camera& cam) override;

	void ClearTileBuffers(const int tileNum, const int layerNum) const;

	void ClearBuffers() override;

	glm::mat4 GetTileTransform(const int tileNum) const;

	unsigned char* GetSpriteData(const char* texturePath) const;

	std::vector<unsigned int> LoadMapData(std::string path) const;


	void CreateFramebuffer(const int layerNum, const int tileNum);

	void BindFramebuffer(const int layerNum, const int tileNum);

	void UnbindFramebuffer(const int layerNum, const int tileNum);

	void RescaleFramebuffer(float width, float height, const int layerNum, const int tileNum);

	unsigned int GetFrameBufferTextures(const int layerNum, const int tileNum);

private:
	unsigned int width, height;

	std::vector<unsigned int> tileMapData;

	Sprite tileMapSprite;

	float vertices[32];
	unsigned int indices[6];

	glm::mat4 transform;

	struct layer{
		std::vector<unsigned int> VAOs;
		std::vector<unsigned int> VBOs;
		std::vector<unsigned int> EBOs;

		std::vector<unsigned int> FBOs;
		std::vector<unsigned int> RBOs;

		std::vector<unsigned int> textures;

		std::vector<unsigned int> frameBufferTextures;

		std::vector<unsigned int> layerData;

		layer(int width, int height, std::vector<unsigned int> data) {
			int size = width * height;
			VAOs.assign(size, 0);
			VBOs.assign(size, 0);
			EBOs.assign(size, 0);
			FBOs.assign(size, 0);
			RBOs.assign(size, 0);
			textures.assign(size, 0);
			frameBufferTextures.assign(size, 0);
			layerData = data;
		}
	};

	std::vector<layer> layers;
};