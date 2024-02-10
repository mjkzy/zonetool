#include "stdafx.hpp"
#include "IW4/Assets/RawFile.hpp"

namespace ZoneTool
{
	namespace IW3
	{
		IW4::RawFile* convert(RawFile* asset, allocator& mem)
		{
			auto* iw4_asset = mem.allocate<IW4::RawFile>();
			iw4_asset->name = asset->name;
			iw4_asset->compressedLen = 0;
			iw4_asset->len = asset->len;
			iw4_asset->buffer = asset->buffer;
			return iw4_asset;
		}

		void IRawFile::dump(RawFile* asset)
		{
			allocator allocator;
			auto* iw4_asset = convert(asset, allocator);

			// dump rawfile
			IW4::IRawFile::dump(iw4_asset);
		}
	}
}