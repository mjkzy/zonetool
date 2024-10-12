#include "stdafx.hpp"
#include "../Include.hpp"

#include "MapEnts.hpp"

namespace ZoneTool::IW5
{
	namespace S1Converter
	{
		S1::MapEnts* GenerateS1MapEnts(MapEnts* asset, allocator& mem)
		{
			// allocate S1 MapEnts structure
			const auto s1_asset = mem.allocate<S1::MapEnts>();

			s1_asset->name = asset->name;

			if (ZoneTool::currentlinkermode == ZoneTool::linker_mode::iw5)
			{
				const auto str = mapents::converter::iw5::convert_mapents_ids(
					std::string{ asset->entityString, static_cast<size_t>(asset->numEntityChars) });
				s1_asset->entityString = const_cast<char*>(mem.duplicate_string(str));
				s1_asset->numEntityChars = static_cast<int>(str.size());
			}
			else
			{
				s1_asset->entityString = asset->entityString;
				s1_asset->numEntityChars = asset->numEntityChars;
			}

			s1_asset->trigger.count = asset->trigger.count;
			s1_asset->trigger.models = reinterpret_cast<S1::TriggerModel*>(asset->trigger.models);
			s1_asset->trigger.hullCount = asset->trigger.hullCount;
			s1_asset->trigger.hulls = reinterpret_cast<S1::TriggerHull*>(asset->trigger.hulls);
			s1_asset->trigger.slabCount = asset->trigger.slabCount;
			s1_asset->trigger.slabs = reinterpret_cast<S1::TriggerSlab*>(asset->trigger.slabs);

			s1_asset->clientTrigger.trigger.count = asset->clientTrigger.trigger.count;
			s1_asset->clientTrigger.trigger.models = reinterpret_cast<S1::TriggerModel*>(asset->clientTrigger.trigger.models);
			s1_asset->clientTrigger.trigger.hullCount = asset->clientTrigger.trigger.hullCount;
			s1_asset->clientTrigger.trigger.hulls = reinterpret_cast<S1::TriggerHull*>(asset->clientTrigger.trigger.hulls);
			s1_asset->clientTrigger.trigger.slabCount = asset->clientTrigger.trigger.slabCount;
			s1_asset->clientTrigger.trigger.slabs = reinterpret_cast<S1::TriggerSlab*>(asset->clientTrigger.trigger.slabs);
			s1_asset->clientTrigger.numClientTriggerNodes = asset->clientTrigger.numClientTriggerNodes;
			s1_asset->clientTrigger.clientTriggerAabbTree = reinterpret_cast<S1::ClientTriggerAabbNode*>(asset->clientTrigger.clientTriggerAabbTree);
			s1_asset->clientTrigger.triggerStringLength = asset->clientTrigger.triggerStringLength;
			s1_asset->clientTrigger.triggerString = asset->clientTrigger.triggerString;

			auto allocate = [&](const short default_value = -1)
			{
				const auto count = asset->clientTrigger.trigger.count;
				auto memory = mem.allocate<short>(asset->clientTrigger.trigger.count);
				std::fill(memory, memory + count, default_value);
				return memory;
			};

			s1_asset->clientTrigger.triggerType = allocate(0);
			s1_asset->clientTrigger.visionSetTriggers = allocate();
			s1_asset->clientTrigger.lightSetTriggers = allocate();

			for (auto i = 0; i < asset->clientTrigger.trigger.count; i++)
			{
				if ((asset->clientTrigger.triggerType[i] & CLIENT_TRIGGER_VISIONSET) != 0)
				{
					s1_asset->clientTrigger.triggerType[i] |= H1::CLIENT_TRIGGER_VISIONSET;

					s1_asset->clientTrigger.visionSetTriggers[i] = asset->clientTrigger.triggerStringOffsets[i];
					s1_asset->clientTrigger.lightSetTriggers[i] = asset->clientTrigger.triggerStringOffsets[i];
				}
			}

			s1_asset->clientTrigger.clutTriggers = allocate(); // todo?
			s1_asset->clientTrigger.origins = reinterpret_cast<float(*__ptr64)[3]>(asset->clientTrigger.origins);
			s1_asset->clientTrigger.scriptDelay = asset->clientTrigger.scriptDelay;
			s1_asset->clientTrigger.audioTriggers = asset->clientTrigger.audioTriggers;
			s1_asset->clientTrigger.blendLookup = allocate(); // todo?
			s1_asset->clientTrigger.npcTriggers = allocate(); // todo?
			s1_asset->clientTrigger.contextTriggers = allocate(); // todo?
			s1_asset->clientTrigger.waterTriggers = allocate(); // todo?

			s1_asset->clientTriggerBlend.numClientTriggerBlendNodes = 0;
			s1_asset->clientTriggerBlend.blendNodes = nullptr;

			// generate spawnents with mapents2spawns
			s1_asset->spawnList.spawnsCount = 0;
			s1_asset->spawnList.spawns = nullptr;

			s1_asset->splineList.splineCount = 0;
			s1_asset->splineList.splines = nullptr;

			return s1_asset;
		}

		S1::MapEnts* convert(MapEnts* asset, allocator& allocator)
		{
			// generate s1 mapents
			return GenerateS1MapEnts(asset, allocator);
		}
	}
}
