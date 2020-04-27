/**
 * @file   DrawTexture.h
 * @author Max Ortner
 * @date   4-15-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "Shader.h"
#include "Drawable.h"
#include "Texture.h"

namespace sgal
{
	class SGAL_API DrawTexture : public Surface, public Sizable, GL::Object
	{
		Vec2u* last_dimensions;

		Texture color;
		
		struct RenderBuffer : GL::Object
		{
			RenderBuffer();
			~RenderBuffer();

			void create();

			void   bind() const override;
			void unbind() const override;
		} depth;

		void   bind() const override;
		void unbind() const override;

	public:
		DrawTexture();
		DrawTexture(const Vec2u& size, bool depth_buffer = true);
		~DrawTexture();

		const Texture& texture() const;

		void render();

		void create(const Vec2u& size, bool depth_buffer = true);
		
		SURFACE_BINDS;
	};
}