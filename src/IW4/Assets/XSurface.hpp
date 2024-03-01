#pragma once

#include "IW5/Structs.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		IW5::XModelSurfs* generate_surface(XModelSurfs* asset, allocator& mem);

		class IXSurface
		{
		public:
			static void dump(XModelSurfs* asset);
		};
	}
}