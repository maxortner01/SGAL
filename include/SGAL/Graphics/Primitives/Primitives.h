#pragma once

#include <SGAL/decl.h>

namespace sgal
{
    class SGAL_API Primitives
    {
        static unsigned char cube[1604];
        static unsigned char outline_cube[1556];
        static unsigned char plane[241];

	public:
		static const VertexArray& getCube();
		static const VertexArray& getOutlineCube();
		static const VertexArray& getPlane();
    };
}

