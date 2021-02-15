#pragma once
#define _XM_NO_INTRINSICS_
#define XM_NO_ALGINMENT
#include <DirectXMath.h>
using namespace DirectX;
#include <vector>

struct PointLight
{
	PointLight() {}
	PointLight(float x, float y, float z)
	{
		XMVectorSetX(pointLightVector, x);
		XMVectorSetX(pointLightVector, y);
		XMVectorSetX(pointLightVector, z);
	}
	XMVECTOR pointLightVector = { 0.0, 0.0, 0.0, 0.0 }; //Default - light starts at 0,0,0

	XMVECTOR colour = { 0.2f, 0.2f, 0.2f, 0.0f };

};

struct DirectionalLight
{
	DirectionalLight() {}
	DirectionalLight(float x, float y, float z)
	{
		XMVectorSetX(directionalLightVector, x);
		XMVectorSetX(directionalLightVector, y);
		XMVectorSetX(directionalLightVector, z);
	}
	XMVECTOR directionalLightVector = { -0.25, 0.5, -0.5, 0.0 }; //Default - light comes down negative Z

	XMVECTOR colour = { 0.6f, 0.6f, 0.0f, 0.0f };
	
};

class LightManager
{
public:
	LightManager();
	~LightManager();

	void UpdateLightManager();

	void CreatePointLight(float x = 0, float y = 0, float z = 0);
	void CreateDirectionalLight();

	PointLight* GetPointLight();
	DirectionalLight* GetDirectionalLight();

private:

	PointLight* pointLight;

	DirectionalLight* directionalLight; //Scene can only have one directional light


};
