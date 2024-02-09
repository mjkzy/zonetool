#include "stdafx.hpp"

#include "FxWorld.hpp"
#include "Converter/H1/Assets/FxWorld.hpp"
#include "H1/Assets/FxWorld.hpp"

namespace ZoneTool::IW5::H1Dumper
{
	void dump(FxWorld* asset)
	{
		// generate h1 fxworld
		allocator allocator;
		auto* h1_asset = H1Converter::convert(asset, allocator);

		// dump h1 fxworld
		H1::IFxWorld::dump(h1_asset);
	}
}