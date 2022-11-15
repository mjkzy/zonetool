#include "stdafx.hpp"

namespace ZoneTool
{
	namespace IW3
	{
		void IRawFile::dump(RawFile* asset, ZoneMemory* mem)
		{
			auto fp = FileSystem::FileOpen(asset->name, "wb");

			if (fp)
			{
				if (asset->len > 0)
				{
					fwrite(asset->buffer, asset->len, 1, fp);
				}
			}

			FileSystem::FileClose(fp);
		}
	}
}