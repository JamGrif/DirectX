#include "LightManager.h"

LightManager::LightManager()
{
	//pointlights.resize(MaxPointLights);
	//pointlights.shrink_to_fit();

}

LightManager::~LightManager()
{
}

void LightManager::UpdateLightManager()
{
	
}

void LightManager::CreatePointLight(float x, float y, float z)
{
	pointLight = new PointLight(x, y, z);

}

void LightManager::CreateDirectionalLight(float x, float y, float z)
{
	directionalLight = new DirectionalLight(x, y, z);
	directionalLight->Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	directionalLight->Diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	directionalLight->Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	//if (directionalLight == nullptr)
	//{
	//	directionalLight = new DirectionalLight(x, y, z);
	//	directionalLight->Ambient = {0.2f, 0.2f, 0.2f, 1.0f};
	//	directionalLight->Diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	//	directionalLight->Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	//
	//}
}


PointLight* LightManager::GetPointLight()
{
	if (pointLight != nullptr)
	{
		return pointLight;
	}
	return nullptr;
}

DirectionalLight* LightManager::GetDirectionalLight()
{
	return directionalLight;
	//if (directionalLight != nullptr)
	//{
	//	return directionalLight;
	//}
	//
	//return nullptr;
}
