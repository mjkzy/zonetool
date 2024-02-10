#pragma once
#include "H1/Structs.hpp"

namespace ZoneTool::H1
{
	namespace physworld_gen
	{
		PhysWorld* generate_physworld(clipMap_t* asset, allocator* mem);
	}
}