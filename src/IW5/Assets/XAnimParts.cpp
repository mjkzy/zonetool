#include "stdafx.hpp"
#include "H1/Assets/XAnimParts.hpp"

#include "Converter/H1/Assets/XAnimParts.hpp"

namespace ZoneTool::IW5
{
	void IXAnimParts::dump(XAnimParts* asset, ZoneMemory* mem, const std::function<const char* (uint16_t)>& convertToString)
	{
		// generate h1 anims
		auto h1_asset = Converter::convert(asset, mem);

		// dump h1 anims
		H1::IXAnimParts::dump(h1_asset, convertToString);
	}
}