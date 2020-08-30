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

			Backspace = 0x08,
			Tab       = 0x09,
			Escape    = 0x1B,
			Space     = 0x20,

			Left  = 0x25,
			Up    = 0x26,
			Right = 0x27,
			Down  = 0x28,

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

			A = 0x41,
			B = 0x42,
			C = 0x43,
			D = 0x44,
			E = 0x45,
			F = 0x46,
			G = 0x47,
			H = 0x48,
			I = 0x49,
			J = 0x4A,
			K = 0x4B,
			L = 0x4C,
			M = 0x4D,
			N = 0x4E,
			O = 0x4F,
			P = 0x50,
			Q = 0x51,
			R = 0x52,
			S = 0x53,
			T = 0x54,
			U = 0x55,
			V = 0x56,
			W = 0x57,
			X = 0x58,
			Y = 0x59,
			Z = 0x5A,

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

			Multiply  = 0x6A,
			Add       = 0x6B,
			Seperator = 0x6C,
			Subtract  = 0x6D,
			Decimal   = 0x6E,
			Divide    = 0x6F,

			LSHIFT = 0xA0,
			RSHIFT = 0xA1,
			LCTRL  = 0xA2,
			RCTRL  = 0xA3

#		endif
		};

		static bool isKeyPressed(KeyCode key);
	};
}