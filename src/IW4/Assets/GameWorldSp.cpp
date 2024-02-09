#include "stdafx.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		void IGameWorldSp::dump(GameWorldSp* asset)
		{
			allocator allocator;

			// GameWorldSp is not 1:1 with GameWorldMp, but we don't need the SP data
			auto* mp_asset = allocator.allocate<GameWorldMp>();
			memset(mp_asset, 0, sizeof GameWorldMp);

			mp_asset->name = asset->name;
			mp_asset->g_glassData = asset->g_glassData;

			IGameWorldMp::dump(mp_asset);
		}
	}
}