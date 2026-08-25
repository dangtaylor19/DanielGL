
class Module;

class Light : public Module {
public:
	Light() {
		this->name = "Light";

		this->radius = 5;
		this->intensity = 1;
		this->color = Vector3(1,1,1);
	}

	void OnPlay() override;
	void OnUpdate() override;

	void DrawInspector() override;

	void Serialize(ofstream& ofstream) const override;
	void Deserialize(ifstream& ifstream) const override;

	float radius;
	float intensity;
	Vector3 color;

private:

};