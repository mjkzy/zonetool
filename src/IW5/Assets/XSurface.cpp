#include "stdafx.hpp"
#include "H1/Assets/XSurface.hpp"

#include "Converter/H1/Assets/XSurface.hpp"

namespace ZoneTool::IW5
{
	void IXSurface::dump(ModelSurface* asset, ZoneMemory* mem)
	{
		// generate h1 surfaces
		auto h1_asset = Converter::convert(asset, mem);

		// dump h1 surfaces
		H1::IXSurface::dump(h1_asset);
	}
}