#include "stdafx.hpp"

#include "GlassWorld.hpp"
#include "Converter/H1/Assets/GlassWorld.hpp"
#include "H1/Assets/GlassWorld.hpp"

namespace ZoneTool::IW5::H1Dumper
{
	void dump(GlassWorld* asset)
	{
		// generate h1 glassworld
		allocator allocator;
		auto* h1_asset = H1Converter::convert(asset, allocator);

		// dump h1 glassworld
		H1::IGlassWorld::dump(h1_asset);
	}
}