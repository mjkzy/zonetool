#include "stdafx.hpp"
#include "IW5/Assets/GfxLightDef.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		void IGfxLightDef::dump(GfxLightDef* asset)
		{
			allocator allocator;
			auto iw5_asset = allocator.allocate<IW5::GfxLightDef>();

			// copy data
			memcpy(iw5_asset, asset, sizeof GfxLightDef);
			memset(&iw5_asset->cucoloris, 0, sizeof GfxLightImage);
			iw5_asset->lmapLookupStart = asset->lmapLookupStart;

			// dump data
			IW5::IGfxLightDef::dump(iw5_asset);
		}
	}
}