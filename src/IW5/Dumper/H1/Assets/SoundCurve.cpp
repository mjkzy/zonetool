#include "stdafx.hpp"

#include "SoundCurve.hpp"
#include "Converter/H1/Assets/SoundCurve.hpp"
#include "H1/Assets/SoundCurve.hpp"

namespace ZoneTool::IW5::H1Dumper
{
	void dump(SndCurve* asset)
	{
		// generate h1 asset
		allocator allocator;
		auto* h1_asset = H1Converter::convert(asset, allocator);

		// dump soundcurve
		H1::ISoundCurve::dump(h1_asset);
	}
}