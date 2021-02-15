#include "Scene.h"

Scene::Scene()
	:floor(nullptr), sphere(nullptr), pointy(nullptr)
{
	sceneCamera = new Camera();

	lightmanager = new LightManager();
	EngineStatics::SetLightManager(lightmanager);
	//lightmanager->CreateDirectionalLight();
	//lightmanager->CreatePointLight(0.0f, -3.0f, -4.0f);


	sceneMeshes.push_back(floor = new Mesh());
	floor->LoadMesh((char*)"res/mesh/cube.obj");
	floor->AddTexture((char*)"res/textures/box2.png");
	//floor->SetRasterizerState(RDefault);
	floor->SetYPos(-4);
	floor->SetZPos(10);
	floor->SetXScale(15);
	floor->SetZScale(15);

	sceneMeshes.push_back(sphere = new Mesh());
	sphere->LoadMesh((char*)"res/mesh/sphere.obj");
	sphere->AddTexture((char*)"res/textures/box.png");
	//sphere->SetRasterizerState(RDefault);
	sphere->SetYPos(0);
	sphere->SetXPos(-4);
	sphere->SetZPos(8);
	sphere->SetXScale(0.5);
	sphere->SetYScale(0.5);
	sphere->SetZScale(0.5);
	
	
	
	sceneMeshes.push_back(pointy = new Mesh());
	pointy->LoadMesh((char*)"res/mesh/cube.obj");
	pointy->AddTexture((char*)"res/textures/swag.png");
	//pointy->SetRasterizerState(RDefault);
	pointy->SetYPos(0);
	pointy->SetZPos(10);
	pointy->SetXScale(1.5);
	pointy->SetYScale(1.5);
	pointy->SetZScale(1.5);




}

Scene::~Scene()
{

}

void Scene::UpdateScene()
{
	sceneCamera->Update();

	lightmanager->UpdateLightManager();



	//pointy->IncYRot(150 * EngineStatics::GetGameTimer()->DeltaTime());

	
	//for (Mesh* m : sceneMeshes)
	//{
	//	if (m != nullptr)
	//	{
	//		m->Update();
	//	}
	//	
	//	
	//}

	if (floor != nullptr) floor->Update();
	if (sphere != nullptr) sphere->Update();
	if (pointy != nullptr) { pointy->Update();  }
	//if (pointy != nullptr) { pointy->Update(); pointy->IncXRot(0.01); pointy->IncYRot(0.01); }

	
	

	if (Input::IsKeyPressed(DIK_G))
	{
		delete pointy; 
		pointy = nullptr;

		//delete sphere;
		//sphere = nullptr;
		//
		//delete floor;
		//floor = nullptr;
		//sceneMeshes.erase(std::remove(sceneMeshes.begin(), sceneMeshes.end(), pointy), sceneMeshes.end());
	}

	if (Input::IsKeyPressed(DIK_H))
	{
		//delete pointy; 
		//pointy = nullptr;

		delete sphere;
		sphere = nullptr;
		//
		//delete floor;
		//floor = nullptr;
		//sceneMeshes.erase(std::remove(sceneMeshes.begin(), sceneMeshes.end(), pointy), sceneMeshes.end());
	}

	if (Input::IsKeyPressed(DIK_J))
	{
		//delete pointy; 
		//pointy = nullptr;

		//delete sphere;
		//sphere = nullptr;
		//
		delete floor;
		floor = nullptr;
		//sceneMeshes.erase(std::remove(sceneMeshes.begin(), sceneMeshes.end(), pointy), sceneMeshes.end());
	}
}
