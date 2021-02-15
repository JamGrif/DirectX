#include "CoreLooper.h"

CoreLooper::CoreLooper(HINSTANCE* hInst, HWND* hWnd)
	:enginestatics(nullptr), input(nullptr), renderer(nullptr)
{
	//Create EngineStatics
	coreObjects.push_back(enginestatics = new EngineStatics());

	//Set approriate values in EngineStatics
	EngineStatics::SetHWND(hWnd);
	EngineStatics::SetHINSTANCE(hInst);

	RECT rc;
	GetClientRect(*EngineStatics::GetHWND(), &rc); 
	EngineStatics::SetScreenWidth(rc.right - rc.left);
	EngineStatics::SetScreenHeight(rc.bottom-rc.top);


	//Create input manager
	coreObjects.push_back(input = new Input());

	//Create renderer
	coreObjects.push_back(renderer = new Renderer());


}

CoreLooper::~CoreLooper()
{
	if (enginestatics)
	{
		delete enginestatics;
		enginestatics = nullptr;
	}

	if (input)
	{
		delete input;
		input = nullptr;
	}

	if (renderer)
	{
		delete renderer;
		renderer = nullptr;
	}
}

void CoreLooper::UpdateCore()
{
	for (CoreObject* co : coreObjects)
	{
		co->Update();
	}
}
