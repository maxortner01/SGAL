#pragma once

#include <SGAL/decl.h>

#include "Event.h"
#include "Context.h"

#include <stack>
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
		friend class Context;

		bool          _open;
		VideoSettings settings;
		Context       context;

	public:
		Window(VideoSettings videoSettings);
		~Window();

		bool isOpen() const;
		void close();

		VideoSettings getVideoSettings() const;

		void clear() const;
		void update();

		void pushEvent(Event  event);
		bool poll     (Event& event);
	};
}