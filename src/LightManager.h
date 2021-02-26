#pragma once

#include <windows.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALGINMENT
#include <DirectXMath.h>
using namespace DirectX;
#include <vector>


struct DirectionalLight
{
	DirectionalLight(float x = 0.0, float y = 0.0, float z = 0.0)
	{
		ZeroMemory((this), sizeof(this));
		Direction.x = x;
		Direction.y = y;
		Direction.z = z;
	}
	
	//Default 
	XMFLOAT4 Ambient	= XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	XMFLOAT4 Diffuse	= XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	XMFLOAT4 Specular	= XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	XMFLOAT3 Direction;
	float Packing; //Constant buffer packing

};

struct PointLight
{
	PointLight(float x = 0.0, float y = 0.0, float z = 0.0)
	{
		ZeroMemory((this), sizeof(this));
		Position.x = x;
		Position.y = y;
		Position.z = z;
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	//Fitted into 4D vector (Position, Range)
	XMFLOAT3 Position;
	float Range; //Range of light. A point whose distance from light source is greater then range is not lit

	//Fitted into 4D vector (Attenuation, Packing)
	XMFLOAT3 Attenuation; //Stores attenuation constants that control how light intensity falls off with distance
	float Packing; //Constant buffer packing

};

struct SpotLight
{
	SpotLight(float x = 0.0, float y = 0.0, float z = 0.0)
	{
		ZeroMemory((this), sizeof(this));
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	//Packed into 4D vector (Position, Range)
	XMFLOAT3 Position;
	float Range; //Range of light. A point whose distance from light source is greater then range is not lit

	//Packed into 4D vector (Direction, Spot)
	XMFLOAT3 Direction;
	float Spot; //Exponent used to calculate the spotlight cone

	//Packed into 4D vector (Attenuation, Packing)
	XMFLOAT3 Attenuation; //Stores attenuation constants that control how light intensity falls off with distance
	float Packing; //Constant buffer packing
};

class LightManager
{
public:
	LightManager();
	~LightManager();

	void UpdateLightManager();

	void CreatePointLight(float x = 0, float y = 0, float z = 0);
	void CreateDirectionalLight(float x = 0.0, float y = 0.0, float z = 0.0);

	PointLight* GetPointLight();
	DirectionalLight* GetDirectionalLight();

private:

	PointLight* pointLight;

	DirectionalLight* directionalLight; //Scene can only have one directional light


};
