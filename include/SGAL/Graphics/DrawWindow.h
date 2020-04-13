/**
 * @file   DrawWindow.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "Window.h"
#include "Surface.h"

namespace sgal
{
	class SGAL_API DrawWindow : public Window, public Surface
	{
	public:
		BASE_CONSTRUCTOR(Window);
	};
}