#include <EngineHeaders.h>
#include <DanielHeaders.h>


glm::mat4 Camera::GetView() const { return this->view; }

glm::mat4 Camera::GetProjection() const { return this->projection; }

void Camera::SetPosition(const Vector3 &pos) {
	this->position = pos;
	UpdateCameraVectors();
	this->view = glm::lookAt(
		glm::vec3(position.x, position.y, position.z),
		glm::vec3(position.x, position.y, position.z) + forward,
		up
	);
}

Vector3 Camera::GetPosition() const {
	return this->position;
}

void Camera::SetRotation(const Vector3 &rotation) {
	this->rotation = rotation;
	UpdateCameraVectors();
	this->view = glm::lookAt(
		glm::vec3(position.x, position.y, position.z),
		glm::vec3(position.x, position.y, position.z) + forward,
		up
	);
}

Vector3 Camera::GetRotation() const{
	return this->rotation;
}

void Camera::SetProjection() {
	this->projection = glm::mat4(1.0f);
	if (perspective) {
		this->projection = glm::perspective(glm::radians(45.0f), (width != 0 || height != 0 ? (float)width / (float)height : 1), 0.1f, 1000.0f);
	}
	else {
		this->projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 1000.0f);
	}
}

void Camera::SetAspect(const int width, const int height) {
	this->width = width;
	this->height = height;
}

void Camera::UpdateCameraVectors() {
	glm::vec3 f;
	f.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	f.y = sin(glm::radians(rotation.x));
	f.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	forward = glm::normalize(f);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, f));
}

Vector3 Camera::GetForwardVec(){
	return Vector3(this->forward.x, this->forward.y, this->forward.z);
}

Vector3 Camera::GetUpVec(){
	return Vector3(this->up.x, this->up.y, this->up.z);
}

Vector3 Camera::GetRightVec(){
	return Vector3(this->right.x, this->right.y, this->right.z);
}
