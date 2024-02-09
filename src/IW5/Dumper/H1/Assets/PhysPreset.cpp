#include "stdafx.hpp"

#include "PhysPreset.hpp"
#include "Converter/H1/Assets/PhysPreset.hpp"
#include "H1/Assets/PhysPreset.hpp"

namespace ZoneTool::IW5::H1Dumper
{
	void dump(PhysPreset* asset)
	{
		// generate h1 asset
		allocator allocator;
		auto* h1_asset = H1Converter::convert(asset, allocator);

		// dump asset
		H1::IPhysPreset::dump(h1_asset);
	}
}