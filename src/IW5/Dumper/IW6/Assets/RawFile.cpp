#include "stdafx.hpp"
#include "RawFile.hpp"

#include "Converter/IW6/Assets/RawFile.hpp"
#include "IW6/Assets/RawFile.hpp"

namespace ZoneTool::IW5::IW6Dumper
{
	void dump(RawFile* asset)
	{
		// generate IW6 rawfile
		allocator allocator;
		auto* iw6_asset = IW6Converter::convert(asset, allocator);

		// dump rawfile
		IW6::IRawFile::dump(iw6_asset);
	}
}