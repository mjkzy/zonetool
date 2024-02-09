#include "stdafx.hpp"

#include "XSurface.hpp"
#include "Converter/H1/Assets/XSurface.hpp"
#include "H1/Assets/XSurface.hpp"

namespace ZoneTool::IW5::H1Dumper
{
	void dump(XModelSurfs* asset)
	{
		// generate h1 surfaces
		allocator allocator;
		auto h1_asset = H1Converter::convert(asset, allocator);

		// dump h1 surfaces
		H1::IXSurface::dump(h1_asset);
	}
}