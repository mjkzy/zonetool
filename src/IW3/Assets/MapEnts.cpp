#include "stdafx.hpp"
#include "IW4/Assets/MapEnts.hpp"

namespace ZoneTool
{
	namespace IW3
	{
		IW4::MapEnts* GenerateIW4MapEnts(MapEnts* asset, allocator& mem)
		{
			auto* mapents = mem.allocate<IW4::MapEnts>();
			memset(mapents, 0, sizeof(IW4::MapEnts));

			mapents->name = asset->name;
			mapents->entityString = asset->entityString;
			mapents->numEntityChars = asset->numEntityChars;

			mapents->trigger.count = asset->trigger.count;
			mapents->trigger.models = reinterpret_cast<IW4::TriggerModel*>(asset->trigger.models);
			mapents->trigger.hullCount = asset->trigger.hullCount;
			mapents->trigger.hulls = reinterpret_cast<IW4::TriggerHull*>(asset->trigger.hulls);
			mapents->trigger.slabCount = asset->trigger.slabCount;
			mapents->trigger.slabs = reinterpret_cast<IW4::TriggerSlab*>(asset->trigger.slabs);

			mapents->stageCount = 1;
			mapents->stageNames = mem.allocate<IW4::Stage>();
			mapents->stageNames[0].name = (char*)mem.duplicate_string("stage 0");
			mapents->stageNames[0].triggerIndex = 0x400;
			mapents->stageNames[0].sunPrimaryLightIndex = 0x1;

			return mapents;
		}

		void IMapEnts::dump(MapEnts* asset)
		{
			// generate iw4 mapents
			allocator allocator;
			auto new_asset = GenerateIW4MapEnts(asset, allocator);

			// dump iw4 mapents
			IW4::IMapEnts::dump(new_asset);
		}
	}
}