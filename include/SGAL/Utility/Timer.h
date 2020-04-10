/**
 * @file   Timer.h
 * @author Max Ortner
 * @date   4-9-2020
 * 
 * This class uses the ctime header to keep track of elapsed time.
 * 
 * @copyright Copyright (c) 2020
**/
#pragma once

#include <ctime>

#include <SGAL/decl.h>

namespace sgal
{
	class SGAL_API Timer
	{
		clock_t time;

	public:
		Timer();

		void  restart();
		float getElapsed() const;
	};
}