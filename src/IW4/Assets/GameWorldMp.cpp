#include "stdafx.hpp"
#include "IW5/Assets/GlassWorld.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		void IGameWorldMp::dump(GameWorldMp* asset)
		{
			IW5::IGlassWorld::dump(reinterpret_cast<IW5::GlassWorld*>(asset));
		}
	}
}