/**
 * @file   Texture.h
 * @author Max Ortner
 * @date   4-13-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "GL.h"
#include "Math.h"

namespace sgal
{
	class SGAL_API Texture : public GL::Object
	{
		unsigned int* layer;
		Vec2u size;

	public:
		enum Type
		{
			Color,
			Depth
		};

		Texture(unsigned int _layer = 0);
		Texture(Vec2u dimensions, Type type = Color, unsigned int _layer = 0);

		~Texture();

		void create(Vec2u dimensions, Type type = Color);
		void fromFile(const std::string& filename);

		Vec2u getSize() const;

		void   bind() const;
		void unbind() const;

		unsigned int getLayer() const;
		void         setLayer(unsigned int _layer) const;
	};
}