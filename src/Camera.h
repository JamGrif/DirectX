#pragma once
#include "EngineStatics.h"
#include "Input.h"

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
    Camera();
    ~Camera();

    void Update();

	void Rotate(float xOffset, float yOffset);
	void Forward(float distance); 
	void Strafe(float distance);

	XMFLOAT3 GetPosition();
	XMMATRIX GetViewMatrix();


private:

	//m_dx is front vector
	float m_x, m_y, m_z, m_dx, m_dy, m_dz, m_camera_rotation;
	float YAW, PITCH;

	//lookat is the position you aim the camera at. Set to be a distance of 1 from the camera (in whatever direction camera is pointing)
	XMVECTOR m_position, m_lookat, m_up;

	XMMATRIX view;



};

/*
Camera(float x, float y, float z)
{
	//m_x = x;
	//m_y = y;
	//m_z = z;
	//YAW = 0.0f;
	//PITCH = 0.0f;
	//m_dx = sin(m_camera_rotation * (XM_PI / 180.0));
	//m_dy = sin(PITCH * (XM_PI / 180.0));
	//m_dz = cos(m_camera_rotation * (XM_PI / 180.0));
	//m_d~ is distance in the x direction from the camera position to the look at point
	//add m_d~ to camera position to get look at point.
}


void Rotate(float xOffset, float yOffset)
{
	YAW += xOffset;
	PITCH -= yOffset;

	//Make sure that when pitch is out of bounds, screen doesnt get flipped
	if (PITCH > 89.0f)
	{
		PITCH = 89.0;
	}
	if (PITCH < -89.0f)
	{
		PITCH = -89.0f;
	}

}

void Forward(float Distance)
{
	m_x += m_dx * Distance;
	m_z += m_dz * Distance;
}

void Strafe(float Distance)
{
	//Calculate forward vector
	XMVECTOR forward = XMVector3Normalize(m_lookat - m_position);

	//Calculate right vector (normalised perpendicular vector to forward and up)
	XMVECTOR up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(forward, up));

	m_x += XMVectorGetX(right) * Distance;
	m_z += XMVectorGetZ(right) * Distance;
}

void Up(float Distance)
{
	m_y += Distance;
}

XMMATRIX GetViewMatrix()
{
	m_dx = sin(YAW * (XM_PI / 180.0)) * cos(PITCH * (XM_PI / 180.0));
	m_dy = sin(PITCH * (XM_PI / 180.0));
	m_dz = cos(YAW * (XM_PI / 180.0)) * cos(PITCH * (XM_PI / 180.0));

	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	m_lookat = XMVectorSet(m_x + m_dx, m_y + m_dy, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	//Takes 3 vectors and returns a view matrix
	return XMMatrixLookAtLH(
		m_position, //Camera position
		m_lookat,   //Direction camera is looking
		m_up		//Which way is 'up' for the camera
	);
}


private:

	//m_dx is front vector
	float m_x, m_y, m_z, m_dx, m_dy, m_dz, m_camera_rotation;
	float YAW, PITCH;

	//lookat is the position you aim the camera at. Set to be a distance of 1 from the camera (in whatever direction camera is pointing)
	XMVECTOR m_position, m_lookat, m_up;


	*/