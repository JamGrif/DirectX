#include "Scene.h"

Scene::Scene()
	:floor(nullptr), sphere(nullptr), pointy(nullptr)
{
	sceneCamera = new Camera();

	lightmanager = new LightManager();
	EngineStatics::SetLightManager(lightmanager);
	lightmanager->CreateDirectionalLight(1.0, 0.0, 0.0);
	//lightmanager->CreatePointLight(0.0f, -3.0f, -4.0f);


	sceneMeshes.push_back(floor = new Mesh());
	floor->LoadMesh((char*)"res/mesh/cube.obj");
	floor->AddTexture((char*)"res/textures/bricks.png");
	floor->SetYPos(-4);
	floor->SetZPos(10);
	floor->SetXScale(15);
	floor->SetZScale(15);

	sceneMeshes.push_back(sphere = new Mesh());
	sphere->LoadMesh((char*)"res/mesh/sphere.obj");
	sphere->AddTexture((char*)"res/textures/box.png");
	sphere->SetYPos(3);
	sphere->SetXPos(-6);
	sphere->SetZPos(13);
	sphere->SetXScale(0.5);
	sphere->SetYScale(0.5);
	sphere->SetZScale(0.5);
	
	
	
	sceneMeshes.push_back(pointy = new Mesh());
	pointy->LoadMesh((char*)"res/mesh/cube.obj");
	pointy->AddTexture((char*)"res/textures/water.png");
	//pointy->SetRasterizerState(RWireframe);
	pointy->SetYPos(0);
	pointy->SetZPos(10);
	pointy->SetXScale(1.5);
	pointy->SetYScale(1.5);
	pointy->SetZScale(1.5);
	//pointy->SetXRot(45);
	//pointy->SetYRot(45);

	//sceneMeshes.push_back(test1 = new Mesh());
	//test1->AddTexture((char*)"res/textures/example.png");
	//test1->SetXScale(1.5);
	//test1->SetYScale(1.5);
	//test1->SetZScale(1.5);
	//test1->SetXPos(-3);
	//
	//sceneMeshes.push_back(test2 = new Mesh());
	//test2->AddTexture((char*)"res/textures/example.png");
	//test2->SetXScale(1.5);
	//test2->SetYScale(1.5);
	//test2->SetZScale(1.5);
	//test2->SetXPos(3);
	//
	//sceneMeshes.push_back(test3 = new Mesh());
	//test3->AddTexture((char*)"res/textures/example.png");
	//test3->SetXScale(1.5);
	//test3->SetYScale(1.5);
	//test3->SetZScale(1.5);
	//test3->SetZPos(-3);
	//
	//sceneMeshes.push_back(test4 = new Mesh());
	//test4->AddTexture((char*)"res/textures/example.png");
	//test4->SetXScale(1.5);
	//test4->SetYScale(1.5);
	//test4->SetZScale(1.5);
	//test4->SetYPos(-3);
	//
	//sceneMeshes.push_back(test5 = new Mesh());
	//test5->AddTexture((char*)"res/textures/example.png");
	//test5->SetXScale(1.5);
	//test5->SetYScale(1.5);
	//test5->SetZScale(1.5);
	//test5->SetZPos(3);

}

Scene::~Scene()
{

}

void Scene::UpdateScene()
{
	sceneCamera->Update();

	lightmanager->UpdateLightManager();


	//pointy->IncXRot(150 * EngineStatics::GetGameTimer()->DeltaTime());
	//pointy->IncYRot(150 * EngineStatics::GetGameTimer()->DeltaTime());

	
	for (Mesh*& m : sceneMeshes)
	{
		if (m != nullptr)
		{
			m->Update();
		}
		
	}

	//if (floor != nullptr) floor->Update();
	//if (sphere != nullptr) sphere->Update();
	//if (pointy != nullptr) { pointy->Update();  }
	//if (pointy != nullptr) { pointy->Update(); pointy->IncXRot(150 * EngineStatics::GetGameTimer()->DeltaTime()); pointy->IncYRot(-150 * EngineStatics::GetGameTimer()->DeltaTime()); }
	//if (pointy != nullptr) { pointy->Update();  }

	//sphere->IncYRot(150 * EngineStatics::GetGameTimer()->DeltaTime());

	
	

	//if (Input::IsKeyPressed(DIK_G))
	//{
	//	delete pointy; 
	//	pointy = nullptr;
	//
	//	//delete sphere;
	//	//sphere = nullptr;
	//	//
	//	//delete floor;
	//	//floor = nullptr;
	//	//sceneMeshes.erase(std::remove(sceneMeshes.begin(), sceneMeshes.end(), pointy), sceneMeshes.end());
	//}
	//
	//if (Input::IsKeyPressed(DIK_H))
	//{
	//	//delete pointy; 
	//	//pointy = nullptr;
	//
	//	delete sphere;
	//	sphere = nullptr;
	//	//
	//	//delete floor;
	//	//floor = nullptr;
	//	//sceneMeshes.erase(std::remove(sceneMeshes.begin(), sceneMeshes.end(), pointy), sceneMeshes.end());
	//}
	//
	//if (Input::IsKeyPressed(DIK_J))
	//{
	//	//delete pointy; 
	//	//pointy = nullptr;
	//
	//	//delete sphere;
	//	//sphere = nullptr;
	//	//
	//	delete floor;
	//	floor = nullptr;
	//	//sceneMeshes.erase(std::remove(sceneMeshes.begin(), sceneMeshes.end(), pointy), sceneMeshes.end());
	//}
}
