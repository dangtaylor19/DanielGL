#include <DanielHeaders.h>
#include <EngineHeaders.h>
#include <stb_image.h>
#include <iostream>
#include <string>

using namespace std;

void Light::OnPlay() {

}

void Light::OnUpdate() {

}

void Light::DrawInspector() {
	if (ImGui::DragFloat("Range", &radius, 0.05f)) {
	}
	if (ImGui::DragFloat("Intensity", &intensity, 0.05f)) {
	}
	if (ImGui::DragFloat3("Color", &color.x, 0.05f, 0.0f, 1.0f)) {
	}
}

void Light::Serialize(ofstream& ofstream) const {
	ofstream << "Radius: " << radius << "\n";
	ofstream << "Intensity: " << intensity << "\n";
	ofstream << "Color: " << color.x << " " << color.y << " " << color.z << "\n";
}

void Light::Deserialize(ifstream& ifstream) const
{

}
