#include "stdafx.hpp"
#include "H1/Assets/ClipMap.hpp"

#include "Converter/H1/Assets/ClipMap.hpp"

#include "H1/Assets/PhysWorld.hpp"
#include "H1/Utils/PhysWorld/generate.hpp"

namespace ZoneTool::IW5
{
	void IClipMap::dump(clipMap_t* asset, ZoneMemory* mem)
	{
		// generate h1 clipmap
		auto* h1_asset = Converter::convert(asset, mem);

		// dump h1 clipmap
		H1::IClipMap::dump(h1_asset, SL_ConvertToString);

		// dump physmap here too i guess, since it's needed.
		allocator allocator;
		auto* physmap = ZoneTool::H1::physworld_gen::generate_physworld(h1_asset, &allocator);

		H1::IPhysWorld::dump(physmap, SL_ConvertToString);
	}
}