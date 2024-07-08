#include "stdafx.hpp"
#include "../Include.hpp"

#include "GfxLightDef.hpp"

namespace ZoneTool::IW5
{
	namespace IW6Converter
	{
		IW6::GfxLightDef* GenerateIW6GfxLightDef(GfxLightDef* asset, allocator& mem)
		{
			auto* iw6_asset = mem.allocate<IW6::GfxLightDef>();
			iw6_asset->name = asset->name;
			if (asset->attenuation.image && asset->attenuation.image->name && *asset->attenuation.image->name)
			{
				iw6_asset->attenuation.image = mem.allocate<IW6::GfxImage>();
				iw6_asset->attenuation.image->name = asset->attenuation.image->name;
			}
			else
			{
				iw6_asset->attenuation.image = mem.allocate<IW6::GfxImage>();
				iw6_asset->attenuation.image->name = "default";
			}
			iw6_asset->attenuation.samplerState = asset->attenuation.samplerState;
			if (asset->cucoloris.image)
			{
				iw6_asset->cucoloris.image = mem.allocate<IW6::GfxImage>();
				iw6_asset->cucoloris.image->name = asset->cucoloris.image->name;
			}
			iw6_asset->cucoloris.samplerState = asset->cucoloris.samplerState;
			iw6_asset->lmapLookupStart = asset->lmapLookupStart;
			return iw6_asset;
		}

		IW6::GfxLightDef* convert(GfxLightDef* asset, allocator& allocator)
		{
			// generate IW6 lightdef
			return GenerateIW6GfxLightDef(asset, allocator);
		}
	}
}