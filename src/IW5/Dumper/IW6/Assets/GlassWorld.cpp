#include "stdafx.hpp"

#include "GlassWorld.hpp"
#include "Converter/IW6/Assets/GlassWorld.hpp"
#include "IW6/Assets/GlassWorld.hpp"

namespace ZoneTool::IW5::IW6Dumper
{
	void dump(GlassWorld* asset)
	{
		// generate IW6 glassworld
		allocator allocator;
		auto* iw6_asset = IW6Converter::convert(asset, allocator);

		// dump IW6 glassworld
		IW6::IGlassWorld::dump(iw6_asset);
	}
}