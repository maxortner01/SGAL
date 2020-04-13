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

#include "../Graphics/Math.h"

namespace sgal
{
	// Since Mouse.h is included in Window.h through Event.h
	class Window;

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

		static void showCursor(bool show);

		static Vec2i getPosition();
		static Vec2i getPosition(const Window& window);

		static void setPosition(Vec2i position);
		static void setPosition(Vec2i position, const Window& window);
	};
}