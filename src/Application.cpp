#include "Application.h"

namespace 
{
	//Forward Windows messages from global MainWndProc to member functions WndProc. This is because member functions can not be assigned to .lpfnWndProc
	Application* gApp = 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gApp->WndProc(hwnd, msg, wParam, lParam);
}

Application::Application(HINSTANCE hInstance, int nCmdShow)
	:m_hInst(hInstance),  m_hWnd(NULL),     m_nCmdShow(nCmdShow), FirstScene(nullptr), 
	 corelooper(nullptr), m_WndMade(false), m_AppPaused(false),   m_Minimized(false),
	 m_Maximized(false),  m_Resizing(false)
{
	//Pointer to application object allows us to forward Window messages to objects WndProc through the global MainWndProc
	gApp = this;
}

Application::~Application()
{
	if (corelooper)
	{
		delete corelooper;
		corelooper = nullptr;
	}
}

bool Application::Start()
{
	//Create window
	{
		//App name
		char WndClassName[100] = "WindowClass";

		//Register class
		WNDCLASSEX wc = { 0 }; //Register a large structure window class and initialises all elements to 0
		wc.cbSize			= sizeof(WNDCLASSEX);														// Size, in bytes, of struct
		wc.style = CS_HREDRAW | CS_VREDRAW;																// These flags indicate the window should be repainted when either horizontal or vertical window size changes.
		wc.lpfnWndProc = MainWndProc;																	// Pointer to window proc function to associate with this WNDCLASS instance.
		wc.cbClsExtra = 0;																				// Extra memory slots that can be used for own purpose. This program does not require any more
		wc.cbWndExtra = 0;																				// ^
		wc.hInstance = m_hInst;																			// Handle to the application instance
		wc.hIcon = (HICON)LoadImage(NULL, "res/icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);				// Specify handle to an icon for the window 
		wc.hIconSm = (HICON)LoadImage(NULL, "res/icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);														// Specify handle to a cursor to use when cursor is over windows client area.
		wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);											// Specify handle to a brush which specifies background color for client area. GetStockObject returns handle to a prebuilt white coloured brush
		wc.lpszMenuName = 0;																			// Specify windows menu. 0 as no menu in application
		wc.lpszClassName = WndClassName;																// Specify name of the window class structure being created

		//Registers the window class
		if (!RegisterClassEx(&wc))
		{
			MessageBox(0, "RegisterClass FAILED", 0, 0);
			return false;
		}

		//Create window
		RECT rc = { 0, 0, m_DEFAULT_SCREENWIDTH, m_DEFAULT_SCREENHEIGHT };	//Creates a rectangle that is used for size of window
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);					//Resize window to take window borders into account

		//Return value is the handle assigned to the new window
		m_hWnd = CreateWindow(
			WndClassName,			// Name of registered WNDCLASS instance to use	
			g_TitleName,			// Window title	
			WS_OVERLAPPEDWINDOW,	// Style flags. Overlappedwindow is combination of several flags (WS_Overlapped, WS_caption, WS_sysmenu, WS_thickframe, WS_Minimizebox, WS_maximizebox)
			CW_USEDEFAULT,			// x-coordinate start of window on screen. CW_USEDEFAULT chooses an appropriate default
			CW_USEDEFAULT,			// y-coordinate ^
			rc.right - rc.left,		// Width of window 
			rc.bottom - rc.top,		// Height of window
			NULL,					// Parent window. Handle to parent window but this window has no relation with other windows so 0
			NULL,					// Menu handle. Program doesnt use any menus so 0
			m_hInst,				// Handle to the application  the window will be associated with
			NULL					// extra creation parameters. Pointer to user-defined data that will be available to WM_CREATE message handler.
		); 

		if (!m_hWnd)
		{
			MessageBox(0, "CreateWindow FAILED", 0, 0);
			return false;
		}
			

		//Show window
		ShowWindow(m_hWnd, m_nCmdShow);
		UpdateWindow(m_hWnd); //Sends WM_PAINT message
		m_WndMade = true;
	} 

	//Create CoreLooper
	corelooper = new CoreLooper(&m_hInst, &m_hWnd);

	//Create Scene
	FirstScene = new Scene();

	return true;
}

int Application::AppLoop()
{
	//Main message loop
	MSG msg = { 0 };

	EngineStatics::GetGameTimer()->Reset();

	while (msg.message != WM_QUIT) //Loop looks for messages on event queue
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //If message relevant to app is found, it is removed from event queue
		{
			//Translate and sent to application windows procedure
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			EngineStatics::GetGameTimer()->Tick();

			if (!m_AppPaused)
			{
				CalculateFrameStats();
				GameLoop();
			}
		}
	}

	return (int)msg.wParam;
}

void Application::GameLoop()
{
	FirstScene->UpdateScene();
	corelooper->UpdateCore();

}

LRESULT Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_WndMade) //Stops any messages being processed before window has been made
	{
		return DefWindowProc(hWnd, message, wParam, lParam); //Pass message on to the default message handling routines
	}

	switch (message)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE) //App deactivated
		{
			m_AppPaused = true;
			EngineStatics::GetGameTimer()->Stop();
		}
		else //App activated
		{
			m_AppPaused = false;
			EngineStatics::GetGameTimer()->Start();
		}
		return 0;

	case WM_SIZE: //Called when window is resized
		if (wParam == SIZE_MINIMIZED)
		{
			m_AppPaused = true;
			m_Minimized = true;
			m_Maximized = false;
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			m_AppPaused = false;
			m_Minimized = false;
			m_Maximized = true;
			EngineStatics::SetScreenWidth(LOWORD(lParam));
			EngineStatics::SetScreenHeight(HIWORD(lParam));
			EngineStatics::GetRenderer()->Resize();
		}
		else if (wParam == SIZE_RESTORED)
		{
			//Restoring from minimized state
			if (m_Minimized)
			{
				m_AppPaused = false;
				m_Minimized = false;
				EngineStatics::SetScreenWidth(LOWORD(lParam));
				EngineStatics::SetScreenHeight(HIWORD(lParam));
				EngineStatics::GetRenderer()->Resize();
			}
			//Restoring from maximized state
			else if (m_Maximized)
			{
				m_AppPaused = false;
				m_Maximized = false;
				EngineStatics::SetScreenWidth(LOWORD(lParam));
				EngineStatics::SetScreenHeight(HIWORD(lParam));
				EngineStatics::GetRenderer()->Resize();
			}
			else if (m_Resizing)
			{

			}
			//API calls
			else 
			{
				EngineStatics::SetScreenWidth(LOWORD(lParam));
				EngineStatics::SetScreenHeight(HIWORD(lParam));
				EngineStatics::GetRenderer()->Resize();
			}
		}
		return 0;

	case WM_ENTERSIZEMOVE: //Called when user grabs resize bars
		m_AppPaused = true;
		m_Resizing = true;
		EngineStatics::GetGameTimer()->Stop();

		return 0;

	case WM_EXITSIZEMOVE: //Called when user releases resize bars
		m_AppPaused = false;
		m_Resizing = false;
		EngineStatics::GetGameTimer()->Start();
		EngineStatics::GetRenderer()->Resize();

		return 0;

	case WM_KEYDOWN:
		if (wParam == 0x1B) DestroyWindow(m_hWnd);

		return 0;

	//case WM_PAINT: //Draws window background
	//	PAINTSTRUCT ps;
	//	HDC hdc;
	//	hdc = BeginPaint(hWnd, &ps);
	//	EndPaint(hWnd, &ps);
	//	return 0;

	case WM_DESTROY: //Called when window is being destroyed
		PostQuitMessage(0);
		
		return 0;

	case WM_MENUCHAR: //Called when menu becomes active and user pressed a key that does not correspond to any mnemonic or accelerator key

		return MAKELRESULT(0, MNC_CLOSE); //Don't beep when alt-enter

	case WM_GETMINMAXINFO: //Called to catch if window becomes too small
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 600;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 600;

		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam); //Pass message on to the default message handling routines
	}
}



void Application::CalculateFrameStats()
{
	//Computes average FPS and average time it takes to render one frame
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	//Compute averages over one second period
	if ((EngineStatics::GetGameTimer()->TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; //fps = frameCnt / 1
		float mspf = 1000.0 / fps;
		std::wostringstream outs;
		outs.precision(6);
		outs << m_MainWndCaption << L"     " << L"FPS: " << fps << L"     " << L"Frame Time: " << mspf << L" (ms)";
		std::string test = "FPS: " + std::to_string(frameCnt);
		SetWindowText(m_hWnd, test.c_str());

		//Reset for next average
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

