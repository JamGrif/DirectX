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

	Mesh* test1;
	Mesh* test2;
	Mesh* test3;
	Mesh* test4;
	Mesh* test5;

	LightManager* lightmanager;

};

