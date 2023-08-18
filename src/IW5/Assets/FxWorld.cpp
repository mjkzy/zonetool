#include "stdafx.hpp"
#include "H1/Assets/FxWorld.hpp"

#include "Converter/H1/Assets/FxWorld.hpp"

namespace ZoneTool::IW5
{
	void IFxWorld::dump(FxWorld* asset, ZoneMemory* mem)
	{
		// generate h1 fxworld
		auto* h1_asset = Converter::convert(asset, mem);

		// dump h1 fxworld
		H1::IFxWorld::dump(h1_asset);
	}
}