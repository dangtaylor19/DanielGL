#include <EngineHeaders.h>

class Camera {
public:
	Camera() : position(Vector3(0,0,-3)), rotation(Vector3(0,0,0)), perspective(true), width(800), height(600) {}

	Camera(Vector3 pos, Vector3 rot, bool perspective, int width, int height) : position(pos), rotation(rot), perspective(perspective), width(width), height(height){
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		worldUp = up;

		forward = glm::vec3(0.0f, 0.0f, 1.0f);
		right = glm::vec3(1.0f, 0.0f, 0.0f);
		
		this->view = glm::lookAt(
			glm::vec3(position.x, position.y, position.z),
			glm::vec3(position.x, position.y, position.z) + forward,
			up
		);

		UpdateCameraVectors();
		SetProjection();
	}

	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;

	void SetProjection();

	void SetPosition(const Vector3& position);
	Vector3 GetPosition() const;

	void SetRotation(const Vector3& rotation);
	Vector3 GetRotation() const;

	void SetAspect(const int width, const int height);

	void UpdateCameraVectors();

	Vector3 GetForwardVec();
	Vector3 GetUpVec();
	Vector3 GetRightVec();

private:
	int width, height;

	bool perspective;

	Vector3 position;
	Vector3 rotation;

	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	glm::mat4 view;
	glm::mat4 projection;
};