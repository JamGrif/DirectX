#pragma once
#include <vector>
#include <algorithm>
#include "LightManager.h"
#include "EngineStatics.h"

#include "Camera.h"

#include "Mesh.h"


class Scene
{
public:

	Scene();
	~Scene();

	void UpdateScene();

private:

	std::vector<Mesh*> sceneMeshes;

	Camera* sceneCamera;

	//Player* player;

	Mesh* floor;
	Mesh* sphere;
	Mesh* pointy;

	LightManager* lightmanager;

};

