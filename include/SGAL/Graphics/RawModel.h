/**
 * @file   RawModel.h
 * @author Max Ortner
 * @date   4-12-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <vector>

#include <SGAL/decl.h>

#include "GL.h"

namespace sgal
{
	class Model;
	class VertexArray;

	/**
	 * @brief Represents the raw data that makes up a model in the GPU.
	 * 
	 * For each unique model there should exist only one RawModel, while any instance that should
	 * be rendered into the scene should be a Model.
	 */
	class SGAL_API RawModel : public GL::ArrayObject
	{
		friend class Model;
		friend class ModelArray;

		std::vector<const Texture*> textures;
		bool use_textures;

	public:
		RawModel();
		RawModel(const VertexArray& array);

		void fromArray(const VertexArray& array);
		void fromFile(const std::string& filename);

		void setColor(const Color& color) const;

		void calculateNormals() const;
		void calculateNormals(const Vec3f* vertices, const size_t vertexCount, const unsigned int* indices = nullptr, const size_t indexCount = 0) const;
		void drawNormals(const Surface* surface, const RenderContext* rc = nullptr);

		void setRenderContext(const RenderContext* rc, const Sizable* surface, const Shader* default_shader = nullptr) const;

		void   bindTextures() const;
		void unbindTextures() const;

		void removeTexture(const Texture& texture);
		void attachTexture(const Texture& texture);
		const std::vector<const Texture*>& getTextures() const;

		unsigned int getByteSize() const;
	};
}