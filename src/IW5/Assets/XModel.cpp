#include "stdafx.hpp"
#include "H1/Assets/XModel.hpp"

#include "Converter/H1/Assets/XModel.hpp"

namespace ZoneTool::IW5
{
	void IXModel::dump(XModel* asset, ZoneMemory* mem, const std::function<const char* (uint16_t)>& convertToString)
	{
		// generate h1 model
		auto h1_asset = Converter::convert(asset, mem);

		// dump h1 model
		H1::IXModel::dump(h1_asset, convertToString);
	}
}