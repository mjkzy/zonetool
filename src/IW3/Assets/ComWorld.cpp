#include "stdafx.hpp"
#include "IW4/Assets/ComWorld.hpp"

namespace ZoneTool::IW3
{
	void IComWorld::dump(ComWorld* asset)
	{
		IW4::IComWorld::dump(reinterpret_cast<IW4::ComWorld*>(asset));
	}
}