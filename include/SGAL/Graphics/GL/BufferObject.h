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

		unsigned int byte_size;
		unsigned int index;
		bool         dynamic, data_bound;

	public:
		BufferObject(const unsigned int _index);
		~BufferObject();

		unsigned int getByteSize() const;

		/**
		 * @brief 
		 * 
		 * @param data    Pointer to the data
		 * @param size    Byte size of each object
		 * @param count   Count of objects
		 * @param members Amount of data members within each object
		 */
		void  loadData(const IPTR data, const size_t size, const size_t count, const size_t members);
		void* readData() const;

		void  bind()  const override;
		void unbind() const override;
	};
}
}