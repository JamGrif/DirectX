#pragma once
//#include <windows.h>
#include <string>
#include <sstream>

#include <vector>

#include "EngineStatics.h"
#include "CoreLooper.h"
#include "Scene.h"

class Application
{
public:
	Application(HINSTANCE hInstance, int nCmdShow);
	~Application();

	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	bool Start();  //Creates window and application objects
	int AppLoop(); //Application loop
	void GameLoop();

	void CalculateFrameStats();

private:
	//Game
	Scene* FirstScene;
	std::vector<Scene*> AllScenes;
	

	//Core
	CoreLooper* corelooper;

	
	//Application
	HINSTANCE	m_hInst;
	HWND		m_hWnd;

	int			m_nCmdShow = NULL;
	char		g_TitleName[16] = "Jamie Griffiths";
	std::wstring m_MainWndCaption = (L"");
	bool m_WndMade;

	//Window size
	UINT m_DEFAULT_SCREENWIDTH = 1280;
	UINT m_DEFAULT_SCREENHEIGHT = 720;

	//Window state
	bool m_AppPaused;	//Is the application paused?
	bool m_Minimized;	//Is the application minimized?
	bool m_Maximized;	//Is the appliocation maximized?
	bool m_Resizing;	//Are the resize bars being dragged?

};

