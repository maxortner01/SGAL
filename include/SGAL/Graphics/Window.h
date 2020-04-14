#pragma once

#include <SGAL/decl.h>

#include "Event.h"
#include "Context.h"
#include "Sizable.h"

#include <stack>
#include <string>

namespace sgal
{
	struct VideoSettings
	{
		unsigned int width, height;
		std::string title;

		HWND_PTR handle;

		VideoSettings(unsigned int _width, unsigned int _height, const std::string& t = "") :
			width(_width), height(_height), title(t), handle(nullptr)
		{   }
	};

	class SGAL_API Window : public Sizable
	{
		friend class Context;

		Vec2i         position;
		bool          _open;
		VideoSettings settings;
		Context       context;

		std::stack<sgal::Event> events;

	public:
		Window(VideoSettings videoSettings);
		~Window();

		bool isOpen() const;
		void close();

		VideoSettings getVideoSettings() const;

		void update();

		void show() const;
		void hide() const;

		void pushEvent(Event  event);
		bool poll     (Event& event);

		void setTitle(const std::string& text) const;
		HWND_PTR getHandle() const;

		void  setPosition(Vec2i pos);
		Vec2i getPosition() const;
	};
}