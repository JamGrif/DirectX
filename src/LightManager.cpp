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
	if (directionalLight == nullptr)
	{
		directionalLight = new DirectionalLight(x, y, z);
	}
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
	if (directionalLight != nullptr)
	{
		return directionalLight;
	}

	return nullptr;
}
