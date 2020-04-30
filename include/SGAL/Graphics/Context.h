/**
 * @file   Context.h
 * @author Max Ortner
 * @date   4-9-2020
 * 
 * This file contains the class used for handling OpenGL contexts.
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

namespace sgal
{
	class SGAL_API Context
	{
		GLRC_PTR handle;
		HDC_PTR  hdc;

	public:
		Context();
		Context(HDC_PTR _hdc);

		~Context();

		void create(HDC_PTR _hdc);

		void makeCurrent() const;
		void swapBuffers() const;
	};
}