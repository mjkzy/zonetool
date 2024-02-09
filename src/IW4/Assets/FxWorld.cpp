#include "stdafx.hpp"
#include "IW5/Assets/FxWorld.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		void IFxWorld::dump(FxWorld* asset)
		{
			IW5::IFxWorld::dump(reinterpret_cast<IW5::FxWorld*>(asset));
		}
	}
}