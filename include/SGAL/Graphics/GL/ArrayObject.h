/**
 * @file   ArrayObject.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "../Math.h"
#include "BufferObject.h"
#include "Object.h"

namespace sgal
{
namespace GL
{
	enum BufferType
	{
		Vertices,
		Normals,
		Colors,
		ModelMatrices,
		NormalMatrices,
		Count
	};

	enum RenderMode
	{
		Triangles,
		Lines,
		Points,
		Polygon
	};

	class SGAL_API ArrayObject : Object
	{
		unsigned int index_count, vertex_count;
		RenderMode   _mode;

		BufferObject* vbos[BufferType::Count];

	protected:
		unsigned int* indices;

	public:
		ArrayObject(RenderMode mode = Triangles, bool dynamic = false);
		~ArrayObject();

		void loadNormalMatrices(const Mat4f* matrices, const size_t count = 1) const;
		void loadModelMatrices (const Mat4f* matrices, const size_t count = 1) const;

		void loadIndices (const unsigned int* _indices, const size_t count = 1);
		void loadVertices(const Vec3f* vertices, const size_t count = 1);
		void loadNormals (const Vec3f* normals, const size_t count = 1) const;
		void loadColors  (const Color* normals, const size_t count = 1) const;

		void setRenderMode(RenderMode mode);
		void setDynamic(bool dynamic) const;

		void   bind() const override;
		void unbind() const override;

		unsigned int vertexCount() const;
		unsigned int indexCount()  const;

		RenderMode    getRenderMode() const;
		BufferObject& operator[](const unsigned int index) const;
	};
}
}