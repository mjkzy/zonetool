#include "stdafx.hpp"
#include "H1/Assets/GlassWorld.hpp"
#include "H1/Assets/FxWorld.hpp"

namespace ZoneTool
{
	namespace IW3
	{
		void IGameWorldMp::dump(GameWorldMp* asset, ZoneMemory* mem)
		{
			// lol, GameWorldMp contains no data in IW3
			auto* h1_gameworld = mem->Alloc<H1::GlassWorld>();
			memset(h1_gameworld, 0, sizeof(H1::GlassWorld));

			h1_gameworld->g_glassData = mem->Alloc<H1::G_GlassData>();
			memset(h1_gameworld->g_glassData, 0, sizeof(H1::G_GlassData));

			h1_gameworld->name = asset->name;

			H1::IGlassWorld::dump(h1_gameworld);

			// dump fx_map here too
			auto* h1_fxworld = mem->Alloc<H1::FxWorld>();
			memset(h1_fxworld, 0, sizeof(H1::FxWorld));

			h1_fxworld->name = asset->name;
			H1::IFxWorld::dump(h1_fxworld);
		}
	}
}