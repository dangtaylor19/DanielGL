#pragma once

#include <string>
#include <Vector3.h>
#include <EngineHeaders.h>
#include <cstring>
#include <Sprite.h>

using namespace std;

class PlayerController{
public:
	PlayerController(GameObject& object) : obj(object){
		this->playerTilePositionX = 0;
		this->playerTilePositionY = 0;

		this->direction = 0;

		this->isMoving = false;
	}

	void UpdatePlayer();

	int GetPosX() const;
	int GetPosY() const;

	void SetPosX(int tilePosX);
	void SetPosY(int tilePosY);

	int GetDirection() const;
	void SetDirection(int direction);

	bool IsMoving();
	void SetMoving(bool moving);

	float GetWaitVal() const;
	void SetWaitVal(float value);

private:
	int playerTilePositionX;
	int playerTilePositionY;

	int direction;

	GameObject& obj;

	bool isMoving;
	float moveWait;
};