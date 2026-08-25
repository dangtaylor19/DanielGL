#include <EngineHeaders.h>
#include <DanielHeaders.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <windows.h>

#include <stdlib.h> 
#include <iostream>
#include <cstring>

#include <fstream>
#include <tinyfiledialogs/tinyfiledialogs.h>

std::ostringstream outputStream;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void DrawObjects(Camera& cam);
void ClearObjects();

void DisplayEntityObject(Entity* entity);

void SaveScene();
void SaveSceneAs();
void OpenScene(string path);

void LoadModelFile(string path);
void ProcessNode(aiNode* node, const aiScene* scene, Entity* previousEntity);

float screenWidth = 800;
float screenHeight = 600;

//Scene camera movement variables
float zoom = -3.0f;
float zoomVal = 10.0f;
float targetZoom = -3.0f;

Vector2 movement;

float camPosX = 0;
float camPosY = 0;
float camSpeed = 0.005f;

std::shared_ptr<Entity> selectedEntity;

auto gizmoOperation = ImGuizmo::TRANSLATE;
auto gizmoSpace = ImGuizmo::LOCAL;

string currentSavePath = "";

Window gameWindow;
Window editorWindow;

Camera editorCamera;
Camera mainCamera; //replace later with pointer to custom camera

bool gamePlaying = false;

//Modules that are rendered
std::vector<std::shared_ptr<Renderer3D>> renderers;
//Entities to be handled
std::vector<std::shared_ptr<Entity>> entities;
//Lights
std::vector<std::shared_ptr<Light>> lights;

Sprite defaultSprite;

//Mesh import
string directory;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	/*Setup console output*/
	std::streambuf* originalCoutBuffer = std::cout.rdbuf();
	std::cout.rdbuf(outputStream.rdbuf());
	
	/*Instantiate GLFW window*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	/*Create window object*/
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "DanielGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
	}
	
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(window);

	/*Initialize GLAD*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	/*Setup window*/
	glViewport(0, 0, bufferWidth, bufferHeight);

	/*Window Icon*/
	GLFWimage img[1];
	img[0].pixels = stbi_load("Assets/wall.jpg", &img[0].width, &img[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(window, 1, img);
	stbi_image_free(img[0].pixels);


	/*Window resizing*/
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	/*Import default shader program*/
	Shader gameShader("engine/core/DefaultVertexShader.vs", "engine/core/DefaultFragmentShader.fs");

	gameWindow = Window();
	gameWindow.create_framebuffer(screenWidth, screenHeight);

	editorWindow = Window();
	editorWindow.create_framebuffer(screenWidth, screenHeight);
	
	//IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	defaultSprite = Sprite("Assets/wall.jpg", 4096, 4096);

	OpenScene("Assets/DefaultScene.scene");

	editorCamera = Camera(Vector3(-3.5f, 2, -3.0f), Vector3(-15.0f, 90.0f, 0.0f), true, screenWidth, screenHeight);
	editorCamera.SetRotation(Vector3(-25.0f, 45.0f, 0.0f));
	mainCamera = Camera(Vector3(0, 0, -3.0f), Vector3(0.0f, 0.0f, 0.0f), true, screenWidth, screenHeight);

	/*Input*/
	glfwSetScrollCallback(window, InputManager::scrollCallback);
	glfwSetMouseButtonCallback(window, InputManager::mouseButtonCallback);
	glfwSetKeyCallback(window, InputManager::keyCallback);
	glfwSetCursorPosCallback(window, InputManager::cursorPositionCallback);

	/*For time delta*/
	float oldTime = 0;

	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{		
		//Update Time
		GameTime::updateDelta(glfwGetTime() - oldTime);
		oldTime = glfwGetTime();

		/*Editor Camera Controls*/
		if (InputManager::GetMouseButton(1)) {
			//if (InputManager::GetMouseScroll("Vertical")) {
			//	targetZoom = zoom + InputManager::GetMouseScroll("Vertical") * zoomVal;
			//}
			//
			//zoom = GMath::Lerp(zoom, targetZoom, 8 * GameTime::delta);

			//Movement
			movement = Vector2(InputManager::GetAxis("Horizontal") * 8, InputManager::GetAxis("Vertical") * 8);

			Vector3 pos = editorCamera.GetPosition();

			pos += editorCamera.GetRightVec() * movement.x * GameTime::delta;
			pos += editorCamera.GetForwardVec() * movement.y * GameTime::delta;

			editorCamera.SetPosition(pos);

			//Rotation
			editorCamera.SetRotation(Vector3(editorCamera.GetRotation().x + InputManager::GetMousePositionDelta().y, editorCamera.GetRotation().y - InputManager::GetMousePositionDelta().x, 0));
		}

		/*Update Input Manager - MUST BE AFTER GAMEPLAY*/
		InputManager::Update();

		/*Transparrency*/
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		//ImGui UI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		/*Render*/
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ImGui::NewFrame();

		ImGuiIO& io = ImGui::GetIO();

		int gameFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
		
		const ImGuiViewport * viewport = ImGui::GetMainViewport();
		ImGui::DockSpaceOverViewport(0, viewport);

		if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O)) {
			const char* path = tinyfd_openFileDialog("Open scene...", "Assets/", 0, nullptr, nullptr, false);

			if (path) {
				currentSavePath = path;
				OpenScene(currentSavePath);
			}
		}else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S)) {
			if (currentSavePath == "") {
				//No current save, open save as instead
				SaveSceneAs();
			}
			else {
				SaveScene();
			}
		}

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New Scene")) {
					OpenScene("Assets/DefaultScene.scene");
					currentSavePath = "";
				}
				if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
					const char* path = tinyfd_openFileDialog("Open scene...", "Assets/", 0, nullptr, nullptr, false);

					if (path) {
						currentSavePath = path;
						OpenScene(currentSavePath);
					}
				}
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
					if (currentSavePath == "") {
						//No current save, open save as instead
						SaveSceneAs();
					}
					else {
						SaveScene();
					}
				}
				if (ImGui::MenuItem("Save Scene as...")) {
					SaveSceneAs();
				}
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Asset")) {
				if (ImGui::MenuItem("Import 3D Model")) {
					const char* path = tinyfd_openFileDialog("Open mode...", "Assets/", 0, nullptr, nullptr, false);

					LoadModelFile(path);
				}
				ImGui::EndMenu();
			}

			float windowWidth = ImGui::GetWindowWidth();

			const char* buttonLabel = gamePlaying ? "Stop" : "Play";
			ImVec2 buttonSize = ImGui::CalcTextSize(buttonLabel);
			buttonSize.x += ImGui::GetStyle().FramePadding.x * 2;
			buttonSize.y += ImGui::GetStyle().FramePadding.y * 2;

			float buttonX = (windowWidth - buttonSize.x) / 2.0f;
			float currentCursorPosX = ImGui::GetCursorPosX();
			ImGui::SetCursorPosX(buttonX);

			if (ImGui::Button(buttonLabel, buttonSize)){
				gamePlaying = !gamePlaying;
			}
			ImGui::EndMainMenuBar();
		}
		
		ImGui::Begin("Game", nullptr);
		{
			//if (ImGui::IsWindowFocused() && gamePlaying) {
			//	std::cout << "handling game input" << std::endl;
			//}
			
			gameWindow.bind_framebuffer();

			const float window_width = ImGui::GetContentRegionAvail().x;
			const float window_height = ImGui::GetContentRegionAvail().y;

			mainCamera.SetAspect(window_width, window_height);
			mainCamera.SetProjection();
			gameWindow.rescale_framebuffer(window_width, window_height);

			glViewport(0, 0, window_width, window_height);

			ImVec2 pos = ImGui::GetCursorScreenPos();

			ImGui::GetWindowDrawList()->AddImage(
				gameWindow.GetTexture(),
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + window_width, pos.y + window_height),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);

			DrawObjects(mainCamera);
		}
		ImGui::End();

		ImGui::Begin("Scene", nullptr);
		{
			//if (ImGui::IsWindowFocused() && !gamePlaying) {
			//	std::cout << "handling scene input" << std::endl;
			//}
			
			const float window_width = ImGui::GetContentRegionAvail().x;
			const float window_height = ImGui::GetContentRegionAvail().y;

			editorCamera.SetAspect(window_width, window_height);
			editorCamera.SetProjection();
			editorWindow.rescale_framebuffer(window_width, window_height);

			glViewport(0, 0, window_width, window_height);

			ImVec2 pos = ImGui::GetCursorScreenPos();

			ImGui::GetWindowDrawList()->AddImage(
				editorWindow.GetTexture(),
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + window_width, pos.y + window_height),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);

			DrawObjects(editorCamera);

			//Gizmos
			if (selectedEntity) {
				if (ImGui::IsKeyPressed(ImGuiKey_E)) {
					gizmoOperation = ImGuizmo::TRANSLATE;
				}
				if (ImGui::IsKeyPressed(ImGuiKey_R)) {
					gizmoOperation = ImGuizmo::ROTATE;
				}
				if (ImGui::IsKeyPressed(ImGuiKey_T)) {
					gizmoOperation = ImGuizmo::SCALE;
				}

				if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
					gizmoSpace = gizmoSpace == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
				}	

				ImGuizmo::BeginFrame();

				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(
					ImGui::GetWindowPos().x,
					ImGui::GetWindowPos().y,
					ImGui::GetWindowWidth(),
					ImGui::GetWindowHeight()
				);

				const float snapping[3] = { 0.1f, 0.1f, 0.1f };

				ImGuizmo::Manipulate(glm::value_ptr(editorCamera.GetView()), glm::value_ptr(editorCamera.GetProjection()),
					gizmoOperation,
					gizmoSpace,
					glm::value_ptr(selectedEntity.get()->GetTransform()),
					0,
					snapping);

				if (ImGuizmo::IsUsing())
				{
					// Decompose matrix back to position/rotation/scale
					glm::vec3 translation, rotation, scale;
					ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(selectedEntity.get()->GetTransform()),
						glm::value_ptr(translation),
						glm::value_ptr(rotation),
						glm::value_ptr(scale));
					selectedEntity.get()->SetPos(Vector3(translation.x, translation.y, translation.z));
					selectedEntity.get()->SetRotation(Vector3(rotation.x, rotation.y, rotation.z));
					selectedEntity.get()->SetScale(Vector3(scale.x, scale.y, scale.z));
				}
			}
		}
		ImGui::End();

		ImGui::Begin("Objects", nullptr);
		{
			if (ImGui::BeginListBox("##ObjectsList", ImGui::GetContentRegionAvail())) {
				//Right click in objects menu
				if (ImGui::BeginPopupContextWindow("ObjectsContextMenu", ImGuiPopupFlags_MouseButtonRight))
				{
					if (ImGui::MenuItem("Create new entity")) {
						auto newEntity = std::make_shared<Entity>();

						entities.push_back(newEntity);

						selectedEntity = newEntity;
					}

					ImGui::EndPopup();
				}

				std::vector<shared_ptr<Entity>> entitiesToRemove;

				std::vector<shared_ptr<Entity>> processedEntities;

				for (const auto& entity : entities)
				{
					if (entity.get()->parent != nullptr) continue;

					DisplayEntityObject(entity.get());

					//Right click on an item
					if (ImGui::BeginPopupContextItem(("ItemPopup##" + std::to_string(entity->ID)).c_str(), ImGuiPopupFlags_MouseButtonRight))
					{
						if (ImGui::MenuItem("Delete")) {
							entitiesToRemove.push_back(entity);
						}
						ImGui::EndPopup();
					}
				}

				//Remove deleted entities
				for (const auto& entity : entitiesToRemove) {
					auto iterator = std::find(entities.begin(), entities.end(), entity);
					if (iterator != entities.end()) {
						entities.erase(iterator);

						for (const auto& module : entity->GetModules()) {
							if (typeid(*module) == typeid(Renderer3D)) {
								auto renderIterator = std::find(renderers.begin(), renderers.end(), module);
								if (renderIterator != renderers.end()) {
									(*renderIterator)->ClearBuffers();
									renderers.erase(renderIterator);
								}
							}
							else if (typeid(*module) == typeid(Light)) {
								auto lightIterator = std::find(lights.begin(), lights.end(), module);
								if (lightIterator != lights.end()) {
									lights.erase(lightIterator);
								}
							}
						}
					}
				}

				ImGui::EndListBox();
			}
		}
		ImGui::End();

		if (ImGui::Begin("Details", nullptr)) {
			if (ImGui::BeginListBox("##DetailsList", ImGui::GetContentRegionAvail())) {
				if (selectedEntity) {
					//Entity name
					char buffer[64];
					strncpy_s(buffer, selectedEntity.get()->name.c_str(), 64);
					if (ImGui::InputText("Entity name", buffer, 64)) {
						selectedEntity.get()->name = buffer;
					}

					//Right click in details menu
					if (ImGui::BeginPopupContextWindow("DetailsContextMenu", ImGuiPopupFlags_MouseButtonRight))
					{
						if (ImGui::MenuItem("Add Renderer")) {
							auto newRenderer = std::make_shared<Renderer3D>(&defaultSprite);
							if (selectedEntity->AddModule(newRenderer)) {
								renderers.push_back(newRenderer);
							}
							newRenderer.get()->InitializeShape();
						}
						if (ImGui::MenuItem("Add Light")) {
							auto newLight = std::make_shared<Light>();
							if (selectedEntity->AddModule(newLight)) {
								lights.push_back(newLight);
							}
						}

						ImGui::EndPopup();
					}

					//Display transform
					if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
						//Position
						Vector3 pos = selectedEntity->GetPos();
						if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) {
							selectedEntity->SetPos(pos);
						}

						//Rotation
						Vector3 rotation = selectedEntity->GetRotation();
						if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f)) {
							selectedEntity->SetRotation(rotation);
						}

						//Scale
						Vector3 scale = selectedEntity->GetScale();
						if (ImGui::DragFloat3("Scale", &scale.x, 0.1f, 0.0f)) {
							selectedEntity->SetScale(scale);
						}
					}

					//Store modules for deletion outside of loop
					std::vector<shared_ptr<Module>> modulesToRemove;

					//Display all the modules in the details panel
					for (const auto& module : selectedEntity->GetModules()) {
						if (ImGui::CollapsingHeader(("Header" + module.get()->name).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
							//Right click on module
							if (ImGui::BeginPopupContextItem((module.get()->name + "ContextMenu##" + std::to_string(selectedEntity->ID)).c_str(), ImGuiPopupFlags_MouseButtonRight)){
								if (ImGui::MenuItem("Delete module")) {
									modulesToRemove.push_back(module);
								}

								ImGui::EndPopup();
							}

							module.get()->DrawInspector();
						}
					}
				
					//Remove modules
					for (const auto& module : modulesToRemove) {
						if (typeid(*module) == typeid(Renderer3D)) {
							auto renderIterator = std::find(renderers.begin(), renderers.end(), module);
							if (renderIterator != renderers.end()) {
								(*renderIterator)->ClearBuffers();
								renderers.erase(renderIterator);
							}
						}
						else if (typeid(*module) == typeid(Light)) {
							auto lightIterator = std::find(lights.begin(), lights.end(), module);
							if (lightIterator != lights.end()) {
								lights.erase(lightIterator);
							}
						}
						else {

						}

						auto moduleIterator = std::find(selectedEntity.get()->GetModules().begin(), selectedEntity.get()->GetModules().end(), module);
						selectedEntity.get()->GetModules().erase(moduleIterator);
					}
				}
			}
			ImGui::EndListBox();
		}
		ImGui::End();

		ImGui::Begin("Assets", nullptr);
		{

		}
		ImGui::End();

		ImGui::Begin("Console", nullptr);
		ImGui::Text(outputStream.str().c_str());
		ImGui::End();

		//outputStream.str("");

		ImGui::Render();


		gameWindow.unbind_framebuffer();
		
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		//Reset Scroll Values
		InputManager::GetMouseScroll("");

		/*Call events and swap buffers*/
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ClearObjects();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	gameWindow.destroy_buffers();
	editorWindow.destroy_buffers();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	editorCamera.SetProjection();
}

void DrawObjects(Camera& cam) {
	for (const auto& object : renderers) {
		if (object) object->Draw(cam, lights);
	}
}

void ClearObjects() {
	for (const auto& object : renderers) {
		if (object) object->ClearBuffers();
	}
}

void DisplayEntityObject(Entity* entity) {
	//No Dropdown
	if (entity->children.size() == 0) {
		if (ImGui::Selectable((entity->name + "##" + std::to_string(entity->ID)).c_str()))
		{
			selectedEntity = entity->shared_from_this();

			std::cout << "selected " + entity->name << std::endl;
		}
	}
	//Dropdown
	else {
		bool treeOpen = ImGui::TreeNodeEx((entity->name + "##Tree" + std::to_string(entity->ID)).c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

		if (ImGui::IsItemClicked()) {
			selectedEntity = entity->shared_from_this();

			std::cout << "selected " + entity->name << std::endl;
		}

		if (treeOpen) {

			for (const auto& childEntity : entity->children) {
				DisplayEntityObject(childEntity.get());
			}
			
			ImGui::TreePop();
		}
	}
}

void SaveSceneAs() {
	const char* path = tinyfd_saveFileDialog("Save file as...", "newScene.scene", 0, nullptr, nullptr);

	if (path) {
		currentSavePath = path;
		SaveScene();
	}
}

void SaveScene() {
	ofstream outputStream(currentSavePath);

	outputStream << "DefaultScene.scene Save File:\n";
	for (const auto& entity : entities) {
		outputStream << "-\n";
		outputStream << "Entity\n";
		outputStream << "Name: " << entity.get()->name << "\n";
		outputStream << "ID: " << entity.get()->ID << "\n";
		outputStream << "Position: " << entity.get()->GetPos().x << " " << entity.get()->GetPos().y << " " << entity.get()->GetPos().z << "\n";
		outputStream << "Rotation: " << entity.get()->GetRotation().x << " " << entity.get()->GetRotation().y << " " << entity.get()->GetRotation().z << "\n";
		outputStream << "Scale: " << entity.get()->GetScale().x << " " << entity.get()->GetScale().y << " " << entity.get()->GetScale().z << "\n";

		for (const auto& module : entity.get()->GetModules()) {
			outputStream << "-\n";
			outputStream << "Module:" << module.get()->name << "\n";
			module.get()->Serialize(outputStream);
		}
	}

	outputStream.close();
	std::cout << "Project Saved!" << std::endl;
}

void OpenScene(string path) {
	//Delete all entities
	auto entitiesToDelete = entities;
	for (const auto& entity : entitiesToDelete) {
		auto iterator = std::find(entities.begin(), entities.end(), entity);
		if (iterator != entities.end()) {
			entities.erase(iterator);

			for (const auto& module : entity->GetModules()) {
				if (typeid(*module) == typeid(Renderer3D)) {
					auto renderIterator = std::find(renderers.begin(), renderers.end(), module);
					if (renderIterator != renderers.end()) {
						(*renderIterator)->ClearBuffers();
						renderers.erase(renderIterator);
					}
				}
				else if (typeid(*module) == typeid(Light)) {
					auto lightIterator = std::find(lights.begin(), lights.end(), module);
					if (lightIterator != lights.end()) {
						lights.erase(lightIterator);
					}
				}
			}
		}
	}

	//Load scene
	ifstream inputStream(path);
	string line;

	Entity* currentEntity = nullptr;

	while (std::getline(inputStream, line)) {
		if (line == "-") {
			//New section
			continue;
		}

		if (line == "Entity") {
			//Read entity
			auto newEntity = std::make_shared<Entity>();
			entities.push_back(newEntity);
			currentEntity = newEntity.get();
		}
		else if (currentEntity) {
			//Read entity data
			if (line.find("Name:") == 0) {
				currentEntity->name = line.substr(6);
			}
			else if (line.find("ID:") == 0) {
				currentEntity->ID = std::stoi(line.substr(4));
			}
			else if (line.find("Position:") == 0) {
				int xIndex = 10;
				int yIndex = line.find(" ", xIndex) + 1;
				int zIndex = line.find(" ", yIndex) + 1;
				currentEntity->SetPos(Vector3(std::stof(line.substr(xIndex, yIndex - xIndex - 1)), std::stof(line.substr(yIndex, zIndex - yIndex - 1)), std::stof(line.substr(zIndex))));
			}
			else if (line.find("Rotation:") == 0) {
				int xIndex = 10;
				int yIndex = line.find(" ", xIndex) + 1;
				int zIndex = line.find(" ", yIndex) + 1;
				currentEntity->SetRotation(Vector3(std::stof(line.substr(xIndex, yIndex - xIndex - 1)), std::stof(line.substr(yIndex, zIndex - yIndex - 1)), std::stof(line.substr(zIndex))));
			}
			else if (line.find("Scale:") == 0) {
				int xIndex = 7;
				int yIndex = line.find(" ", xIndex) + 1;
				int zIndex = line.find(" ", yIndex) + 1;
				currentEntity->SetScale(Vector3(std::stof(line.substr(xIndex, yIndex - xIndex - 1)), std::stof(line.substr(yIndex, zIndex - yIndex - 1)), std::stof(line.substr(zIndex))));
			}
			else if (line.find("Module:" == 0)) {
				//Add new module
				std:string moduleName = line.substr(7);
				if (moduleName == "Renderer") {
					auto newRenderer = std::make_shared<Renderer3D>(&defaultSprite);
					currentEntity->AddModule(newRenderer);
					newRenderer->InitializeShape();

					renderers.push_back(newRenderer);
				}
				else if (moduleName == "Light") {
					auto newLight = std::make_shared<Light>();
					currentEntity->AddModule(newLight);

					lights.push_back(newLight);
				}
				else {

				}
			}
		}
	}

	inputStream.close();
	std::cout << "New Project!" << std::endl;
}

void LoadModelFile(string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene, nullptr);
}

void ProcessNode(aiNode* node, const aiScene* scene, Entity* previousEntity){
	//Create new entity
	auto newEntity = std::make_shared<Entity>();
	newEntity.get()->parent = previousEntity;
	if (previousEntity != nullptr) previousEntity->children.push_back(newEntity);
	entities.push_back(newEntity);

	//Get and set transform of node
	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	node->mTransformation.Decompose(scale , rotation, position);

	newEntity.get()->SetPos(Vector3(position.x, position.y, position.z));
	newEntity.get()->SetRotation(Vector3(rotation.x, rotation.y, rotation.z));
	newEntity.get()->SetScale(Vector3(scale.x, scale.y, scale.z));

	std::shared_ptr<Renderer3D> newRenderer;

	if (previousEntity != nullptr) {
		newRenderer = std::make_shared<Renderer3D>(&defaultSprite);
		newEntity.get()->AddModule(newRenderer);

		newRenderer.get()->SetSprite(&defaultSprite);

		renderers.push_back(newRenderer);
	}

	//process node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		//Set mesh
		newRenderer.get()->mesh.get()->ProcessMesh(mesh);

		newRenderer.get()->InitializeShape();
	}
	//process children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, newEntity.get());
	}
}