#include "stdafx.hpp"
#include "IW5/Assets/GfxImage.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		void IGfxImage::dump(GfxImage* asset)
		{
			IW5::IGfxImage::dump(reinterpret_cast<IW5::GfxImage*>(asset));
		}
	}
}