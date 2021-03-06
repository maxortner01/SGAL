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
		TexCoords,
		ModelMatrices,
		NormalMatrices,
		BufferCount
	};

	enum RenderMode
	{
		Triangles,
		Lines,
		LineStrip,
		Points,
		Polygon
	};

	/**
	 * @brief High-level abstraction of OpenGL's VAO.
	 * 
	 * This object contains BufferObjects within it that each hold various
	 * information which describe a model. There should be one ArrayObject
	 * per unique model used in the SGAL.
	 */
	class SGAL_API ArrayObject : Object
	{
		unsigned int index_count, vertex_count;
		RenderMode   _mode;

		BufferObject* vbos[BufferType::BufferCount];

	protected:
		unsigned int* indices;

	public:
		ArrayObject(RenderMode mode = Triangles, bool dynamic = false);
		~ArrayObject();

		void loadNormalMatrices(const Mat4f* matrices, const size_t count = 1) const;
		void loadModelMatrices (const Mat4f* matrices, const size_t count = 1) const;

		void loadIndices  (const unsigned int* _indices, const size_t count = 1);
		void loadVertices (const Vec3f* vertices, const size_t count = 1);
		void loadNormals  (const Vec3f* normals, const size_t count = 1) const;
		void loadColors   (const Color* normals, const size_t count = 1) const;
		void loadTexCoords(const Vec2f* coords, const size_t count = 1) const;

		void setRenderMode(RenderMode mode);
		void setDynamic(bool dynamic) const;

		void   bind() const override;
		void unbind() const override;


		const unsigned int* getIndices() const;
		unsigned int vertexCount() const;
		unsigned int indexCount()  const;

		RenderMode    getRenderMode() const;
		BufferObject& operator[](const unsigned int index) const;
	};
}
}