#include "stdafx.hpp"

#include "PathData.hpp"
#include "Converter/H1/Assets/PathData.hpp"
#include "H1/Assets/PathData.hpp"

namespace ZoneTool::IW5::H1Dumper
{
	void dump(PathData* asset)
	{
		// generate h1 clipmap
		allocator allocator;
		auto* h1_asset = H1Converter::convert(asset, allocator);

		// dump h1 clipmap
		H1::IPathData::dump(h1_asset, SL_ConvertToString);
	}
}
