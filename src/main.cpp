#include "Application.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	int wParam = NULL;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//Enable runtime memory check for debug builds
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF) | _CRTDBG_LEAK_CHECK_DF;
#endif

	Application* application = new Application(hInstance, nCmdShow);
	if (application->Start())
	{
		wParam = application->AppLoop();
	}

	delete application;

	return wParam;
}