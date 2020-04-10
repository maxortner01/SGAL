/**
 * @file   Mouse.h
 * @author Max Ortner
 * @date   4-10-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

namespace sgal
{
	struct MouseState
	{
		bool left, right, middle;
	};

	class SGAL_API Mouse
	{
	public:
		enum KeyCode
		{
#		ifdef WIN32
			Key_LEFT   = 0x01,
			Key_RIGHT  = 0x02,
			Key_MIDDLE = 0x04,
#		endif
		};

		static MouseState getState();
	};
}