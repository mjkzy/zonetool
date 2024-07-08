#include "stdafx.hpp"
#include "../Include.hpp"

#include "GfxLightDef.hpp"

namespace ZoneTool::IW5
{
	namespace H1Converter
	{
		H1::GfxLightDef* GenerateH1GfxLightDef(GfxLightDef* asset, allocator& mem)
		{
			auto* h1_asset = mem.allocate<H1::GfxLightDef>();
			h1_asset->name = asset->name;
			if (asset->attenuation.image && asset->attenuation.image->name && *asset->attenuation.image->name)
			{
				h1_asset->attenuation.image = mem.allocate<H1::GfxImage>();
				h1_asset->attenuation.image->name = asset->attenuation.image->name;
			}
			else
			{
				h1_asset->attenuation.image = mem.allocate<H1::GfxImage>();
				h1_asset->attenuation.image->name = "default";
			}
			h1_asset->attenuation.samplerState = asset->attenuation.samplerState;
			if (asset->cucoloris.image)
			{
				h1_asset->cucoloris.image = mem.allocate<H1::GfxImage>();
				h1_asset->cucoloris.image->name = asset->cucoloris.image->name;
			}
			h1_asset->cucoloris.samplerState = asset->cucoloris.samplerState;
			h1_asset->lmapLookupStart = asset->lmapLookupStart;
			return h1_asset;
		}

		H1::GfxLightDef* convert(GfxLightDef* asset, allocator& allocator)
		{
			// generate h1 lightdef
			return GenerateH1GfxLightDef(asset, allocator);
		}
	}
}