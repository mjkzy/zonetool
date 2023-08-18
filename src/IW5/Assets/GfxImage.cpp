#include "stdafx.hpp"
#include "H1/Assets/GfxImage.hpp"

#include "Converter/H1/Assets/GfxImage.hpp"

namespace ZoneTool::IW5
{
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

		if (!asset->texture->resourceSize)
		{
			ZONETOOL_INFO("Not dumping image %s", asset->name);
			return;
		}

		// generate h1 gfximage
		auto* h1_asset = Converter::convert(asset, mem);

		// dump h1 gfximage
		H1::IGfxImage::dump(h1_asset);
	}
}