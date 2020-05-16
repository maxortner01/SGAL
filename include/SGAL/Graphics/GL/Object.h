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
	/**
	 * @brief Simple data structure that holds ids used for OpenGL as well as bind/unbind functionality.
	 */
	struct SGAL_API Object
	{
		unsigned int id = 0;

		virtual void   bind() const = 0;
		virtual void unbind() const = 0;
	};
}
}