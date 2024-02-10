#include "stdafx.hpp"

#include "XSurface.hpp"
#include "Converter/IW7/Assets/XSurface.hpp"
#include "IW7/Assets/XSurface.hpp"

namespace ZoneTool::IW5::IW7Dumper
{
	void dump(XModelSurfs* asset)
	{
		// generate IW7 surfaces
		allocator allocator;
		auto iw7_asset = IW7Converter::convert(asset, allocator);

		// dump IW7 surfaces
		IW7::IXSurface::dump(iw7_asset);
	}
}