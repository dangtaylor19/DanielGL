#include <fstream>

class Entity;

class Module {
public:
	//Module() {}

	Module() = default;

	virtual void OnPlay() {}
	virtual void OnUpdate() {}

	virtual void DrawInspector() {}

	virtual void Serialize(ofstream& ofstream) const {}
	virtual void Deserialize(ifstream& ifstream) const {}

	std::weak_ptr<Entity> owner;
	string name;
private:
};