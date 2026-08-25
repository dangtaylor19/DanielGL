#include <EngineHeaders.h>
#include <DanielHeaders.h>
#include <stb_image.h>
#include <iostream>
#include <string>

using namespace std;

void PlayerController::UpdatePlayer() {
	if (IsMoving()) {
		this->obj.SetPos(GMath::MoveTo(this->obj.GetPos(), Vector3(GetPosX(), GetPosY(), 0), 5 * GameTime::delta));
	}

	if ((this->obj.GetPos().x >= GetPosX() - 0.05 && this->obj.GetPos().x <= GetPosX() + 0.05) &&
		(this->obj.GetPos().y >= GetPosY() - 0.05 && this->obj.GetPos().y <= GetPosY() + 0.05)) {
		SetMoving(false);
	}
}

int PlayerController::GetPosX() const{
	return playerTilePositionX;
}

int PlayerController::GetPosY() const {
	return playerTilePositionY;
}

void PlayerController::SetPosX(int tilePosX) {
	this->playerTilePositionX = tilePosX;
}

void PlayerController::SetPosY(int tilePosY) {
	this->playerTilePositionY = tilePosY;
}

int PlayerController::GetDirection() const {
	return this->direction;
}

void PlayerController::SetDirection(int direction) {
	if (direction < 0 || direction > 3){
		std::cout << "ERROR::DIRECTION OUT OF BOUNDS\n" << std::endl;
	};
	this->direction = direction;
	this->obj.SetSpriteIndex(direction * 3);
}

bool PlayerController::IsMoving() {
	return this->isMoving;
}

void PlayerController::SetMoving(bool moving) {
	this->isMoving = moving;
}

float PlayerController::GetWaitVal() const {
	return this->moveWait;
}

void PlayerController::SetWaitVal(float value) {
	this->moveWait = value;
}