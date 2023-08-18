#include "stdafx.hpp"
#include "H1/Assets/ComWorld.hpp"

#include "Converter/H1/Assets/ComWorld.hpp"

namespace ZoneTool::IW5
{
	void IComWorld::dump(ComWorld* asset, ZoneMemory* mem)
	{
		// generate h1 comworld
		auto* h1_asset = Converter::convert(asset, mem);

		// dump h1 comworld
		H1::IComWorld::dump(h1_asset);
	}
}