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
	enum BufferDataType
	{
		Float,
		Int
	};

	/**
	 * @brief High-level abstraction of OpenGL's VBO.
	 * 
	 * This object is used to communicate informtation to-and-fro the GPU.
	 */
	class SGAL_API BufferObject : Object
	{
		friend class ArrayObject;

		unsigned int byte_size;
		unsigned int index;
		bool         dynamic, data_bound;

	public:
		BufferObject(const unsigned int _index = 0);
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
		void  loadData(const IPTR data, const size_t size, const size_t count, const size_t members, const BufferDataType type = Float);
		void* readData() const;

		void   bindBase(unsigned int layer) const;
		void unbindBase(unsigned int layer) const;

		void  bind()  const override;
		void unbind() const override;
	};
}
}