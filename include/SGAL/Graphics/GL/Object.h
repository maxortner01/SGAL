/**
 * @file   Object.h
 * @author Max Ortner
 * @date   4-10-2020
 * 
 * Represents a basic OpenGL object.
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

namespace sgal
{
namespace GL
{
	struct SGAL_API Object
	{
		unsigned int id;

		virtual void   bind() const = 0;
		virtual void unbind() const = 0;
	};
}
}