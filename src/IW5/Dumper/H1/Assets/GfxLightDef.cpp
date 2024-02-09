#include "stdafx.hpp"

#include "GfxLightDef.hpp"
#include "Converter/H1/Assets/GfxLightDef.hpp"
#include "H1/Assets/GfxLightDef.hpp"

namespace ZoneTool::IW5::H1Dumper
{
	void dump(GfxLightDef* asset)
	{
		// generate h1 lightdef
		allocator allocator;
		auto* h1_asset = H1Converter::convert(asset, allocator);

		// dump lightdef
		H1::IGfxLightDef::dump(h1_asset);
	}
}