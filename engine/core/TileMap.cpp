#include <EngineHeaders.h>
#include <DanielHeaders.h>

#include <stb_image.h>
#include <fstream>


void TileMap::InitializeTile(const char* texturePath, const int layerNum, const int tileNum){
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

	unsigned int VBO, VAO, EBO, texture;

	/*Load Texture*/

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	glTexImage2D(GL_TEXTURE_2D, 0, tileMapSprite.GetChannels() == 3 ? GL_RGB : GL_RGBA, 16, 16, 0, tileMapSprite.GetChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, tileMapSprite.GetUVData(this->layers[layerNum].layerData[tileNum]));
	glGenerateMipmap(GL_TEXTURE_2D);

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

	layers[layerNum].VBOs[tileNum] = VBO;
	layers[layerNum].VAOs[tileNum] = VAO;
	layers[layerNum].EBOs[tileNum] = EBO;

	layers[layerNum].textures[tileNum] = texture;
}

void TileMap::DrawTile(const int tileNum, const int layerNum) const{
	glBindTexture(GL_TEXTURE_2D, layers[layerNum].textures[tileNum]);

	glBindVertexArray(layers[layerNum].VAOs[tileNum]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TileMap::Draw(Camera& cam) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < width*height; j++) {
			shader.use();
			shader.setMat4("transform", GetTileTransform(j));
			DrawTile(j, i);
		}
	}
}

void TileMap::ClearTileBuffers(const int tileNum, const int layerNum) const{
	glDeleteVertexArrays(1, &layers[layerNum].VAOs[tileNum]);
	glDeleteBuffers(1, &layers[layerNum].VBOs[tileNum]);
	glDeleteBuffers(1, &layers[layerNum].EBOs[tileNum]);
}

void TileMap::ClearBuffers() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < width * height; j++) {
			ClearTileBuffers(j, i);
		}
	}
}

glm::mat4 TileMap::GetTileTransform(const int tileNum) const {
	glm::mat4 tileTransform = this->transform;

	tileTransform = glm::translate(tileTransform, glm::vec3(tileNum%width,-(int)(tileNum/width),0));
	
	return tileTransform;
}

std::vector<unsigned int> TileMap::LoadMapData(const std::string path) const{
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << path << std::endl;
	}

	std::vector<unsigned int> tileData;
	std::string line;

	while (std::getline(file, line)) {
		std::stringstream substring(line);
		std::string value;

		while (std::getline(substring, value, ',')) {
			tileData.push_back(std::stoi(value));
		}
	}

	file.close();

	return tileData;
}