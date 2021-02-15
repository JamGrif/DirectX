#pragma once

#include "CoreObject.h"
#include "Camera.h"
#include "LightManager.h"
#include "GameTimer.h"
#include "Renderer.h"

class Camera; //Camera forward declaration

class Renderer; //Camera forward declaration

class EngineStatics :
	public CoreObject
{
public:
	EngineStatics();

	void Update() override;

	//Window height
	static void				SetScreenHeight(const int &value)			{ m_CURRENT_SCREENHEIGHT = value; }
	static int				GetScreenHeight()							{ return m_CURRENT_SCREENHEIGHT; }

	//Window width
	static void				SetScreenWidth(const int& value)			{ m_CURRENT_SCREENWIDTH = value; }
	static int				GetScreenWidth()							{ return m_CURRENT_SCREENWIDTH; }
	
	//Window handle
	static void				SetHWND(HWND* winHandle)					{ m_hWnd = winHandle; }
	static HWND*			GetHWND()									{ return m_hWnd; }

	//Window instance
	static void				SetHINSTANCE(HINSTANCE* instanceHandle)		{ m_hInst = instanceHandle; }
	static HINSTANCE*		GetHINSTANCE()								{ return m_hInst; }

	//D3DDevice
	static					ID3D11Device* GetD3DDevice()				{ return m_pD3DDevice; }
	static void				SetD3DDevice(ID3D11Device* device)			{ m_pD3DDevice = device; };

	//D3DContext
	static					ID3D11DeviceContext* GetD3DContext()		{ return m_pImmediateContext; }
	static void				SetD3DContext(ID3D11DeviceContext* context) { m_pImmediateContext = context; }

	//Current camera
	static Camera*			GetCamera()									{ if (m_camera != nullptr) { return m_camera; } else { return nullptr; } }
	static void				SetCamera(Camera* camera)					{ m_camera = camera; }

	//Projection matrix
	static XMMATRIX			GetProjectionMatrix()						{ return m_Projection; }
	static void				SetProjectionMatrix(const XMMATRIX& proj)	{ m_Projection = proj; }

	//Light manager
	static LightManager*	GetLightManager() 							{ return m_lightmanager; }
	static void				SetLightManager(LightManager* lm)			{ m_lightmanager = lm; }

	//Game Timer
	static GameTimer* GetGameTimer()									{ return m_gametimer; }

	//Renderer
	static Renderer* GetRenderer()										{ return m_renderer; }
	static void SetRenderer(Renderer* r)								{ m_renderer = r; }

private:
	static int					m_CURRENT_SCREENHEIGHT;

	static int					m_CURRENT_SCREENWIDTH;

	static HWND*				m_hWnd;

	static HINSTANCE*			m_hInst;

	static ID3D11Device*		m_pD3DDevice;

	static ID3D11DeviceContext* m_pImmediateContext;

	static Camera*				m_camera;

	static XMMATRIX				m_Projection;

	static LightManager*		m_lightmanager;

	static GameTimer*			m_gametimer;

	static Renderer*			m_renderer;

};



#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												   \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTraceW(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 


#define SafeRelease(x) {if(x) {x->Release(); x = nullptr; } }


