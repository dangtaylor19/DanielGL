#include <EngineHeaders.h>

#include <stb_image.h>
#include <vector>
#include <algorithm>

#include <iostream>

#pragma once

class Sprite
{
private:
	const char* texturePath;

	std::vector<std::vector<unsigned char>> textureData;
	unsigned int spriteWidth;
	unsigned int spriteHeight;

	int channels;
	int textureHeight, textureWidth;

public:
	Sprite(const char* texturePath = "Assets/tiles1.png", unsigned int spriteWidth = 16, unsigned int spriteHeight = 16) {
		this->texturePath = texturePath;
		this->spriteWidth = spriteWidth;
		this->spriteHeight = spriteHeight;

		std::vector<unsigned char> empty;
		this->textureData.assign(64, empty);

		/*Load Sprite*/
		int width, height, nrChannels;
		unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

		this->channels = nrChannels;
		this->textureWidth = width;
		this->textureHeight = height;

		//Read image data into textureData
		int startVal = 0;

		for (int y = 0; y < height; y++){
			for (int x = 0; x < width; x++){
				//Add RGBA values
				startVal = ((width - 1 - x) + (y * width)) * nrChannels;
				for (int k = 0; k < nrChannels; k++) {
					textureData[((width - 1 - x) / spriteWidth) + ((static_cast<std::vector<std::vector<unsigned char, std::allocator<unsigned char>>, std::allocator<std::vector<unsigned char, std::allocator<unsigned char>>>>::size_type>(y / spriteHeight)) * (width / spriteWidth))].push_back(data[startVal + k]);
				}
			}
		}

		stbi_image_free(data);
	}

	unsigned char* GetUVData(unsigned int index) const {
		return (unsigned char*)textureData[index].data();
	}

	const char* GetPath() const {
		return texturePath;
	}

	const int GetChannels() const{
		return this->channels;
	}

	const int GetHeight() const {
		return this->textureHeight;
	}

	const int GetWidth() const {
		return this->textureWidth;
	}

	unsigned char* GetAnimatedUV(unsigned int startIndex, unsigned int frames) {
		//std::cout << ((int)(glfwGetTime() * 2) % frames) + startIndex << std::endl;
		return (unsigned char*)textureData[((int)(glfwGetTime() * 2) % frames) + startIndex - 1].data();
	}
};