#include "stdafx.hpp"
#include "../Include.hpp"

#include "RawFile.hpp"

namespace ZoneTool::IW5
{
	namespace IW7Converter
	{
		IW7::RawFile* GenerateIW7RawFile(RawFile* asset, allocator& mem)
		{
			auto* IW7_asset = mem.allocate<IW7::RawFile>();
			IW7_asset->name = asset->name;
			IW7_asset->compressedLen = asset->compressedLen;
			IW7_asset->len = asset->len;
			IW7_asset->buffer = asset->buffer;
			return IW7_asset;
		}

		IW7::RawFile* convert(RawFile* asset, allocator& allocator)
		{
			// generate IW7 rawfile
			return GenerateIW7RawFile(asset, allocator);
		}
	}
}