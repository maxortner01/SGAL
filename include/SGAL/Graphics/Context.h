/**
 * @file   Context.h
 * @author Max Ortner
 * @date   4-9-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/
#pragma once

#include <SGAL/decl.h>

namespace sgal
{
	class SGAL_API Context
	{
		IPTR handle;
		IPTR hdc;

	public:
		Context();
		Context(IPTR _hdc);

		~Context();

		void create(IPTR _hdc);

		void makeCurrent() const;
		void swapBuffers() const;
	};
}