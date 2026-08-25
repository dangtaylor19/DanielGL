#include <string>
#include <Vector3.h>
#include <Module.h>
using namespace std;

class Entity : public std::enable_shared_from_this<Entity> {
public:

	Entity() : Entity("New Entity") {}

	Entity(string name){
		this->ID = nextID++;

		this->name = std::to_string(ID);

		this->position = Vector3(0, 0, 0);
		this->rotation = Vector3(0, 0, 0);
		this->scale = Vector3(1, 1, 1);

		this->transform = glm::mat4(1.0f);
	}

	bool AddModule(std::shared_ptr<Module> module) {
		for (auto& _module : modules) {
			if (typeid(*_module) == typeid(*module)) return false;
		}

		module.get()->owner = shared_from_this();
		modules.push_back(module);

		return true;
	}

	std::vector<std::shared_ptr<Module>>& GetModules() {
		return modules;
	}

	void SetPos(const Vector3& pos);
	Vector3 GetPos() const;

	void SetRotation(const Vector3& rotation);
	Vector3 GetRotation() const;

	void SetScale(const Vector3& scale);
	Vector3 GetScale() const;

	void CalculateTransform();
	glm::mat4& GetTransform();

	/*Variables*/
	static int nextID;
	int ID;
	string name;
	bool isSelected;

	Entity* parent = nullptr;
	std::vector<std::shared_ptr<Entity>> children;

	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale;

private:
	/*Transform*/
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;


	glm::mat4 transform;

	std::vector<std::shared_ptr<Module>> modules;
};