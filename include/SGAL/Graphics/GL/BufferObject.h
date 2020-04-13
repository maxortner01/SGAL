/**
 * @file   BufferObject.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "Object.h"

namespace sgal
{
namespace GL
{
	class SGAL_API BufferObject : Object
	{
		friend class ArrayObject;

		unsigned int index;
		bool         dynamic, data_bound;

	public:
		BufferObject(const unsigned int _index);
		~BufferObject();

		void loadData(const IPTR data, const size_t size, const size_t count, const size_t members);

		void  bind()  const override;
		void unbind() const override;
	};
}
}