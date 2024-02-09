#include "stdafx.hpp"

#include "ComWorld.hpp"
#include "Converter/IW6/Assets/ComWorld.hpp"
#include "IW6/Assets/ComWorld.hpp"

namespace ZoneTool::IW5::IW6Dumper
{
	void dump(ComWorld* asset)
	{
		// generate iw6 comworld
		allocator allocator;
		auto* iw6_asset = IW6Converter::convert(asset, allocator);

		// dump iw6 comworld
		IW6::IComWorld::dump(iw6_asset);
	}
}