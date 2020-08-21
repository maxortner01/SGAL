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
		// Texture Types
	public:
		enum Type
		{
			Color,
			Depth
		};

		enum FilterType
		{
			Nearest,
			Smooth
		};

		enum WrapType
		{
			ClampToEdge,
			Repeat,
			MirroredRepeat
		};

		struct Parameters
		{
			FilterType filter;
			WrapType   wrap;

			// Since gcc struggles with default parameters
			Parameters(FilterType f = FilterType::Nearest, WrapType w = WrapType::ClampToEdge) :
				filter(f), wrap(w)
			{	}
		};

		// Private variables
	private:
		Vec2u      size;
		Parameters parameters;

		// Methods
	public:
		Texture(unsigned int _layer = 0);
		Texture(Vec2u dimensions, Type type = Type::Color, const Parameters& param = Parameters(), unsigned int _layer = 0);
		Texture(Vec2u dimensions, const Parameters& param, unsigned int _layer = 0);

		~Texture();

		void create(Vec2u dimensions, const Parameters& param);
		void create(Vec2u dimensions, Type type = Color, const Parameters& param = Parameters());
		void fromFile(const std::string& filename);
		void fromMemory(const void* const data, const Vec2u& size);

		Parameters getParameters() const;

		Vec2u getSize() const;

		void   bindLayer(const unsigned int layer) const;
		void unbindLayer(const unsigned int layer) const;

		void   bind() const;
		void unbind() const;
	};
}