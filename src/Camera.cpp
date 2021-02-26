#include "Camera.h"

Camera::Camera()
	:m_x(0.0f),m_y(0.0f),m_z(0.0f),m_dx(0.0f),m_dy(0.0f),m_dz(0.0f),PITCH(0.0f),YAW(0.0f), view(XMMatrixIdentity())
{

	EngineStatics::SetCamera(this); //Set this camera to be the current camera
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (Input::IsKeyPressed(DIK_W)) Forward(8.0f * EngineStatics::GetGameTimer()->DeltaTime());
	if (Input::IsKeyPressed(DIK_S)) Forward(-8.0f * EngineStatics::GetGameTimer()->DeltaTime());
	if (Input::IsKeyPressed(DIK_A)) Strafe(8.0f * EngineStatics::GetGameTimer()->DeltaTime());
	if (Input::IsKeyPressed(DIK_D)) Strafe(-8.0f * EngineStatics::GetGameTimer()->DeltaTime());
	Rotate(Input::GetMouseState().lX * 0.2f, Input::GetMouseState().lY * 0.2f);
}

void Camera::Rotate(float xOffset, float yOffset)
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

void Camera::Forward(float distance)
{
	m_x += m_dx * distance;
	m_z += m_dz * distance;
}

void Camera::Strafe(float distance)
{
	//Calculate forward vector
	XMVECTOR forward = XMVector3Normalize(m_lookat - m_position);

	//Calculate right vector (normalised perpendicular vector to forward and up)
	XMVECTOR up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(forward, up));

	m_x += XMVectorGetX(right) * distance;
	m_z += XMVectorGetZ(right) * distance;
}

XMFLOAT3 Camera::GetPosition()
{
	XMFLOAT3 pos = { m_x, m_y, m_z };
	return pos;
}

XMMATRIX Camera::GetViewMatrix()
{
	m_dx = float(sin(YAW * (XM_PI / 180.0)) * cos(PITCH * (XM_PI / 180.0)));
	m_dy = float(sin(PITCH * (XM_PI / 180.0)));
	m_dz = float(cos(YAW * (XM_PI / 180.0)) * cos(PITCH * (XM_PI / 180.0)));

	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	m_lookat = XMVectorSet(m_x + m_dx, m_y + m_dy, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	view = XMMatrixLookAtLH(
		m_position, //Camera position
		m_lookat,   //Direction camera is looking
		m_up		//Which way is 'up' for the camera
	);
	//Takes 3 vectors and returns a view matrix
	return view;
}
