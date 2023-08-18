#include "stdafx.hpp"
#include "H1/Assets/GfxWorld.hpp"

#include "Converter/H1/Assets/GfxWorld.hpp"

namespace ZoneTool::IW5
{
	void IGfxWorld::dump(GfxWorld* asset, ZoneMemory* mem)
	{
		// generate h1 gfxworld
		auto* h1_asset = Converter::convert(asset, mem);

		// dump h1 gfxworld
		H1::IGfxWorld::dump(h1_asset);
	}
}