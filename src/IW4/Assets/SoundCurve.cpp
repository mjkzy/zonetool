#include "stdafx.hpp"
#include "IW5/Assets/SoundCurve.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		void ISoundCurve::dump(SndCurve* asset)
		{
			// dump asset
			IW5::ISoundCurve::dump(reinterpret_cast<IW5::SndCurve*>(asset));
		}
	}
}