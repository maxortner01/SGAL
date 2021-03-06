/**
 * @file   Shader.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>
#include <string>

#include "GL.h"
#include "Math.h"
#include "LightArray.h"

namespace sgal
{
	class Texture;

	class SGAL_API Shader : public GL::Object
	{
	public:
		enum ShaderType
		{
			Vertex,
			Fragment,
			Compute,
			SG_GLSL,
			Count	
		};

	private:
		unsigned int ids[ShaderType::Count];
		std::string  source[ShaderType::Count];

	public:
		Shader();
		~Shader();

		void setUniform(const Light* lights, const size_t count = 1) const;
		void setUniform(const Light& light, const unsigned int index = 0) const;
        void setUniform(const std::string& name, const Texture& texture) const;
        void setUniform(const std::string& name, Mat4f value) const;
        void setUniform(const std::string& name, Color value) const;
        void setUniform(const std::string& name, Vec3f value) const;
        void setUniform(const std::string& name, Vec2f value) const;
        void setUniform(const std::string& name, float value) const;
        void setUniform(const std::string& name, bool  value) const;
        void setUniform(const std::string& name, int   value) const;

		const std::string& getSource(ShaderType type) const;

		void fromFile  (const std::string& filename, ShaderType type);
		void fromString(const std::string& contents, ShaderType type);
		void link() const;

		void   bind() const override;
		void unbind() const override;

		static void useDefault();

		// [SHADER] insert-decl
		static Shader& Default2D();
		static Shader& Default3D();
		static Shader& DefaultUI();
	};
}