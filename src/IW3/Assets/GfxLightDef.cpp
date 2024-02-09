#include "stdafx.hpp"
#include "IW4/Assets/GfxLightDef.hpp"

namespace ZoneTool
{
	namespace IW3
	{
		void IGfxLightDef::dump(GfxLightDef* asset)
		{
			IW4::IGfxLightDef::dump(reinterpret_cast<IW4::GfxLightDef*>(asset));
		}
	}
}