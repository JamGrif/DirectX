#pragma once
#include <windows.h>
#include <vector>
#include "Renderer.h"
#include "Input.h"
#include "EngineStatics.h"


class CoreLooper
{
public:
	CoreLooper(HINSTANCE* hInst, HWND* hWnd);
	~CoreLooper();

	void UpdateCore();


private:

	std::vector<CoreObject*> coreObjects;

	EngineStatics* enginestatics;
	Input* input;
	Renderer* renderer;

};

