/**
 * @file   RawModel.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "GL.h"

namespace sgal
{
	class SGAL_API RawModel : public GL::ArrayObject
	{
		friend class Model;
		friend class ModelArray;

	public:
		RawModel();
	};
}