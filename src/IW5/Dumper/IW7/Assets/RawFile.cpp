#include "stdafx.hpp"
#include "RawFile.hpp"

#include "Converter/IW7/Assets/RawFile.hpp"
#include "IW7/Assets/RawFile.hpp"

namespace ZoneTool::IW5::IW7Dumper
{
	void dump(RawFile* asset)
	{
		// generate IW7 rawfile
		allocator allocator;
		auto* iw7_asset = IW7Converter::convert(asset, allocator);

		// dump rawfile
		IW7::IRawFile::dump(iw7_asset);
	}
}