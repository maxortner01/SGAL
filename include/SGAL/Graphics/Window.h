#pragma once

#include "../decl.h"

#include <string>

namespace sgal
{
	struct VideoSettings
	{
		unsigned int width;
		unsigned int height;
		std::string title;

		void* handle;

		VideoSettings(unsigned int _width, unsigned int _height, const std::string& t = "") :
			width(_width), height(_height), title(t), handle(nullptr)
		{   }
	};

	class SGAL_API Window
	{
		VideoSettings settings;

	public:
		Window(VideoSettings videoSettings);

		bool isOpen() const;
		void Update() const;

		VideoSettings getVideoSettings() const;
	};
}