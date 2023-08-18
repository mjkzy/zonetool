#include "stdafx.hpp"
#include "H1/Assets/RawFile.hpp"

#include "Converter/H1/Assets/RawFile.hpp"

namespace ZoneTool::IW5
{
	void IRawFile::dump(RawFile* asset, ZoneMemory* mem)
	{
		// generate h1 rawfile
		auto* h1_asset = Converter::convert(asset, mem);

		// dump rawfile
		H1::IRawFile::dump(h1_asset);
	}
}