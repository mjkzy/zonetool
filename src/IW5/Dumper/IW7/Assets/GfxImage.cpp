#include "stdafx.hpp"

#include "GfxImage.hpp"
#include "Converter/IW7/Assets/GfxImage.hpp"
#include "IW7/Assets/GfxImage.hpp"

namespace ZoneTool::IW5::IW7Dumper
{
	void dump(GfxImage* asset)
	{
		std::string name = asset->name;

		bool isMapImage = (name.size() >= 6)
			? ((name.substr(0, 6) == "*light" || name.substr(0, 6) == "*refle" ||
				name == "$outdoor")
				? true
				: false)
			: false;

		if (!isMapImage) return;

		if (!asset->texture.loadDef->resourceSize)
		{
			ZONETOOL_INFO("Not dumping image %s", asset->name);
			return;
		}

		// generate IW7 gfximage
		allocator allocator;
		auto* iw7_asset = IW7Converter::convert(asset, allocator);

		// dump IW7 gfximage
		IW7::IGfxImage::dump(iw7_asset);
	}
}