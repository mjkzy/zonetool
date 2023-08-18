#include "stdafx.hpp"
#include "H1/Assets/GlassWorld.hpp"

#include "Converter/H1/Assets/GlassWorld.hpp"

namespace ZoneTool::IW5
{
	void IGlassWorld::dump(GlassWorld* asset, ZoneMemory* mem)
	{
		// generate h1 glassworld
		auto* h1_asset = Converter::convert(asset, mem);

		// dump h1 glassworld
		H1::IGlassWorld::dump(h1_asset);
	}
}