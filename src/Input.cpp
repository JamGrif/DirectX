#include "Input.h"

unsigned char Input::m_keyboard_keys_state[256];
DIMOUSESTATE  Input::m_mouse_state;
Input::Input()
{
	//Keyboard
	ZeroMemory(m_keyboard_keys_state, sizeof(m_keyboard_keys_state));

	DirectInput8Create(*EngineStatics::GetHINSTANCE(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL); //Create DirectInput object

	m_direct_input->CreateDevice(GUID_SysKeyboard, &m_keyboard_device, NULL); //Create keyboard device
	
	m_keyboard_device->SetDataFormat(&c_dfDIKeyboard); //Set data format of keyboard
	
	m_keyboard_device->SetCooperativeLevel(*EngineStatics::GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); //How device will interact with other applications that use the device via DirectInput
	
	m_keyboard_device->Acquire(); //Ask device to start providing input for application


	//Mouse
	m_direct_input->CreateDevice(GUID_SysMouse, &m_mouse_device, NULL); //Create mouse device

	m_mouse_device->SetDataFormat(&c_dfDIMouse); //Set data format of mouse
	
	m_mouse_device->SetCooperativeLevel(*EngineStatics::GetHWND(), DISCL_FOREGROUND | DISCL_EXCLUSIVE); //How device will interact with other applications that use the device via DirectInput
	
	m_mouse_device->Acquire(); //Ask device to start providing input for application
	
}

Input::~Input()
{
	if (m_keyboard_device)
	{
		m_keyboard_device->Unacquire();
		SafeRelease(m_keyboard_device);
	}

	if (m_mouse_device)
	{
		m_mouse_device->Unacquire();
		SafeRelease(m_mouse_device);
	}

	SafeRelease(m_direct_input);
}

void Input::Update()
{
	HRESULT hr;

	hr = m_keyboard_device->GetDeviceState(sizeof(m_keyboard_keys_state), (LPVOID)&m_keyboard_keys_state); //Get keyboards current state and put into data structure for it

	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard_device->Acquire();
		}
	}

	hr = m_mouse_device->GetDeviceState(sizeof(m_mouse_state), (LPVOID)&m_mouse_state); //Get mouses current state and put into data structure for it
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_mouse_device->Acquire();
		}
	}
}
