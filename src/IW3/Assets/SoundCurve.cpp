#include "stdafx.hpp"
#include "IW4/Assets/SoundCurve.hpp"

namespace ZoneTool
{
	namespace IW3
	{
		void ISoundCurve::dump(SndCurve* asset)
		{
			IW4::ISoundCurve::dump(reinterpret_cast<IW4::SndCurve*>(asset));
		}
	}
}