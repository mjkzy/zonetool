#include "stdafx.hpp"
#include "H1/Assets/SoundCurve.hpp"

#include "Converter/H1/Assets/SoundCurve.hpp"

namespace ZoneTool::IW5
{
	void ISoundCurve::dump(SndCurve* asset, ZoneMemory* mem)
	{
		// generate h1 asset
		auto* h1_asset = Converter::convert(asset, mem);

		// dump lightdef
		H1::ISoundCurve::dump(h1_asset);
	}
}