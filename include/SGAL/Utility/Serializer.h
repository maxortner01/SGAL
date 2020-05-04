/**
 * @file   Serializer.h
 * @author Max Ortner
 * @date   4-29-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include <string>

namespace sgal
{
	class RawModel;
	class VertexArray;

	struct SGAL_API Serializer
	{
		static void saveToFile  (const std::string& filename, const RawModel& rawModel);
		
		static void loadFromFile(const std::string& filename, RawModel& rawModel);
		static void loadFromFile(const std::string& filename, VertexArray& array);
	};
}