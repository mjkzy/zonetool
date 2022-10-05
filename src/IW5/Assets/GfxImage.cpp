#include "stdafx.hpp"
#include "H1/Assets/GfxImage.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		H1::GfxImage* GenerateH1GfxImage(GfxImage* asset, ZoneMemory* mem)
		{
			// allocate H1 GfxImage structure
			const auto h1_asset = mem->Alloc<H1::GfxImage>();

			h1_asset->name = asset->name;
			h1_asset->imageFormat = MFMapDX9FormatToDXGIFormat(asset->texture->format);
			h1_asset->mapType = static_cast<H1::MapType>(asset->mapType);
			h1_asset->sematic = asset->semantic;
			h1_asset->category = asset->category;
			h1_asset->flags = asset->flags;
			h1_asset->dataLen1 = asset->texture->dataSize;
			h1_asset->dataLen2 = asset->texture->dataSize;
			h1_asset->width = asset->width;
			h1_asset->height = asset->height;
			h1_asset->depth = asset->depth;
			h1_asset->numElements = asset->texture->levelCount;
			h1_asset->levelCount = asset->mapType == 5 ? 7 : 1;
			h1_asset->streamed = false;
			h1_asset->pixelData = reinterpret_cast<unsigned char*>(&asset->texture->texture);

			return h1_asset;
		}

		void IGfxImage::dump(GfxImage* asset, ZoneMemory* mem)
		{
			std::string name = asset->name;

			bool isMapImage = (name.size() >= 6)
				? ((name.substr(0, 6) == "*light" || name.substr(0, 6) == "*refle" ||
					name == "$outdoor")
					? true
					: false)
				: false;

			if (!isMapImage) return;

			if (!asset->texture->dataSize)
			{
				ZONETOOL_INFO("Not dumping image %s", asset->name);
				return;
			}

			// generate h1 gfximage
			auto h1_asset = GenerateH1GfxImage(asset, mem);

			// dump h1 gfximage
			H1::IGfxImage::dump(h1_asset);
		}
	}
}