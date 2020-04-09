#pragma once

#include "../decl.h"

namespace sgal
{
	struct VideoSettings
	{
		unsigned int width;
		unsigned int height;

		VideoSettings(unsigned int _width, unsigned int _height) :
			width(_width), height(_height)
		{   }
	};

	class SGAL_API Window
	{
	public:
		Window(VideoSettings videoSettings);
	};
}