/**
 * @file   Keyboard.h
 * @author Max Ortner
 * @date   4-10-2020
 * 
 * This class handles the states of each key.
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

namespace sgal
{
	enum KeyState
	{
		KeyDown,
		KeyHeld,
		KeyUp
	};

	class SGAL_API Keyboard
	{
	public:
		enum KeyCode
		{
#		ifdef WIN32
			// Key codes found at https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

			Key_BACKSPACE = 0x08,
			Key_TAB       = 0x09,
			Key_ESCAPE    = 0x1B,
			Key_SPACE     = 0x20,

			Key_LEFT  = 0x25,
			Key_UP    = 0x26,
			Key_RIGHT = 0x27,
			Key_DOWN  = 0x28,

			Key_0 = 0x30,
			Key_1 = 0x31,
			Key_2 = 0x32,
			Key_3 = 0x33,
			Key_4 = 0x34,
			Key_5 = 0x35,
			Key_6 = 0x36,
			Key_7 = 0x37,
			Key_8 = 0x38,
			Key_9 = 0x39,

			Key_A = 0x41,
			Key_B = 0x42,
			Key_C = 0x43,
			Key_D = 0x44,
			Key_E = 0x45,
			Key_F = 0x46,
			Key_G = 0x47,
			Key_H = 0x48,
			Key_I = 0x49,
			Key_J = 0x4A,
			Key_K = 0x4B,
			Key_L = 0x4C,
			Key_M = 0x4D,
			Key_N = 0x4E,
			Key_O = 0x4F,
			Key_P = 0x50,
			Key_Q = 0x51,
			Key_R = 0x52,
			Key_S = 0x53,
			Key_T = 0x54,
			Key_U = 0x55,
			Key_V = 0x56,
			Key_W = 0x57,
			Key_X = 0x58,
			Key_Y = 0x59,
			Key_Z = 0x5A,

			Key_NUMPAD0 = 0x60,
			Key_NUMPAD1 = 0x61,
			Key_NUMPAD2 = 0x62,
			Key_NUMPAD3 = 0x63,
			Key_NUMPAD4 = 0x64,
			Key_NUMPAD5 = 0x65,
			Key_NUMPAD6 = 0x66,
			Key_NUMPAD7 = 0x67,
			Key_NUMPAD8 = 0x68,
			Key_NUMPAD9 = 0x69,

			Key_MULTIPLY  = 0x6A,
			Key_ADD       = 0x6B,
			Key_SEPERATOR = 0x6C,
			Key_SUBTRACT  = 0x6D,
			Key_DECIMAL   = 0x6E,
			Key_DIVIDE    = 0x6F,

			Key_LSHIFT = 0xA0,
			Key_RSHIFT = 0xA1,
			Key_LCTRL  = 0xA2,
			Key_RCTRL  = 0xA3

#		endif
		};

		static bool isKeyPressed(KeyCode key);
	};
}