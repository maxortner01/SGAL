/**
 * @file   VertexArray.h
 * @author Max Ortner
 * @date   5-2-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <vector>

#include <SGAL/decl.h>

#include "Math.h"

namespace sgal
{
	struct Vertex
	{
		Vec3f position, normal;
		Vec2f texture_coord;
		Color color;
	};

	class SGAL_API VertexArray
	{
		uint32_t iterator;
		uint32_t c_size;

		Vertex* vertices;

	public:
		VertexArray(const uint32_t _size = 1);
		VertexArray(const VertexArray& va);
		~VertexArray();

		void resize(const uint32_t _size);
		void push(Vertex& vertex);
		void fit();
		void clear();

		void calculateNormals();
		void append(const VertexArray& array);

		VertexArray& operator=(const VertexArray& va);

		template<typename... Args>
		VertexArray transform(const Mat4f& first, Args... transformations) const;
		VertexArray transform(const Mat4f& transformation) const;

		VertexArray index(const unsigned int* indices, const uint32_t size) const;

		bool typeFilled(GL::BufferType type) const;

		Vertex& get(const uint32_t index) const;
		Vertex& operator[] (const uint32_t index) const;

		uint32_t allocated() const;
		uint32_t size() const;
		
		std::vector<Vec2f> getTexCoords() const;
		std::vector<Vec3f> getVertices()  const;
		std::vector<Vec3f> getNormals()   const;
		std::vector<Color> getColors()	  const;
	};

	template<typename... Args>
	VertexArray VertexArray::transform(const Mat4f& first, Args... transformations) const
	{
		return transform(first).transform(transformations...);
	}
}