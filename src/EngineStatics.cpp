#include "EngineStatics.h"

//Static variable definitions used to establish the need for a piece of memory, which the compiler then reserves

HWND* EngineStatics::m_hWnd									= nullptr;
HINSTANCE* EngineStatics::m_hInst							= nullptr;

int EngineStatics::m_CURRENT_SCREENWIDTH					= 0;
int EngineStatics::m_CURRENT_SCREENHEIGHT					= 0;

ID3D11Device* EngineStatics::m_pD3DDevice					= nullptr;
ID3D11DeviceContext* EngineStatics::m_pImmediateContext		= nullptr;

Camera* EngineStatics::m_camera								= nullptr;

XMMATRIX EngineStatics::m_Projection						= XMMatrixIdentity();

LightManager* EngineStatics::m_lightmanager					= nullptr;

GameTimer* EngineStatics::m_gametimer						= nullptr;

Renderer* EngineStatics::m_renderer							= nullptr;


EngineStatics::EngineStatics()
{
	m_gametimer = new GameTimer();
}

void EngineStatics::Update()
{

}


