#include "stdafx.hpp"

#include "ComWorld.hpp"
#include "Converter/H1/Assets/ComWorld.hpp"
#include "H1/Assets/ComWorld.hpp"

namespace ZoneTool::IW5::H1Dumper
{
	void dump(ComWorld* asset)
	{
		// generate h1 comworld
		allocator allocator;
		auto* h1_asset = H1Converter::convert(asset, allocator);

		// dump h1 comworld
		H1::IComWorld::dump(h1_asset);
	}
}