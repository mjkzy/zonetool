#include "stdafx.hpp"
#include "H1/Assets/PhysPreset.hpp"

#include "Converter/H1/Assets/PhysPreset.hpp"

namespace ZoneTool::IW5
{
	void IPhysPreset::dump(PhysPreset* asset, ZoneMemory* mem)
	{
		// generate h1 asset
		auto* h1_asset = Converter::convert(asset, mem);

		// dump asset
		H1::IPhysPreset::dump(h1_asset);
	}
}