#include "stdafx.hpp"
#include "H1/Assets/GfxLightDef.hpp"

#include "Converter/H1/Assets/GfxLightDef.hpp"

namespace ZoneTool::IW5
{
	void IGfxLightDef::dump(GfxLightDef* asset, ZoneMemory* mem)
	{
		// generate h1 lightdef
		auto* h1_asset = Converter::convert(asset, mem);

		// dump lightdef
		H1::IGfxLightDef::dump(h1_asset);
	}
}