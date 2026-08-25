#include <DanielHeaders.h>

int Entity::nextID = 0;

void Entity::SetPos(const Vector3& pos) {
	this->position = pos;
	if (parent != nullptr) {
		this->localPosition = pos - parent->GetPos();
	}
	else {
		this->localPosition = pos;
	}

	//Set child positions
	for (const auto& child : children){
		child.get()->SetPos(this->position + child.get()->localPosition);
	}

	CalculateTransform();
}
Vector3 Entity::GetPos() const { return this->position; }

void Entity::SetRotation(const Vector3& rotation) {
	this->rotation = rotation;
	if (parent != nullptr) {
		this->localRotation = rotation - parent->GetRotation();
	}
	else {
		this->localRotation = rotation;
	}

	//Set child positions
	for (const auto& child : children) {
		child.get()->SetRotation(this->rotation + child.get()->localRotation);
	}

	CalculateTransform();
}
Vector3 Entity::GetRotation() const { return this->rotation; }

void Entity::SetScale(const Vector3& scale) {
	this->scale = scale;
	if (parent != nullptr) {
		this->localScale = scale - parent->GetScale();
	}
	else {
		this->localScale = scale;
	}

	//Set child positions
	for (const auto& child : children) {
		child.get()->SetScale(this->scale + child.get()->localScale);
	}

	CalculateTransform();
}
Vector3 Entity::GetScale() const { return this->scale; }

glm::mat4& Entity::GetTransform() {
	return this->transform;
}

void Entity::CalculateTransform() {
	this->transform = glm::translate(
		glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z))
		* glm::mat4_cast(glm::quat(glm::radians(glm::vec3(rotation.x, rotation.y, rotation.z))))
		* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, scale.z));
}