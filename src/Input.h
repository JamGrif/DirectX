#pragma once
#include "CoreObject.h"
#include "EngineStatics.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>

class Input :
    public CoreObject
{
public:
    Input();
    ~Input();

    void Update() override;

    static bool IsKeyPressed(unsigned char DI_keycode) { return m_keyboard_keys_state[DI_keycode] & 0x80; }
    static DIMOUSESTATE GetMouseState() { return m_mouse_state; }

private:
    IDirectInput8*          m_direct_input;

    //Keyboard
    IDirectInputDevice8*    m_keyboard_device;
    static unsigned char    m_keyboard_keys_state[256];

    //Mouse
    IDirectInputDevice8*    m_mouse_device;
    static DIMOUSESTATE     m_mouse_state;

};

