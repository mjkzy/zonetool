#include "stdafx.hpp"
#include "../Include.hpp"

#include "MapEnts.hpp"

namespace ZoneTool::IW5
{
	namespace H1Converter
	{
		H1::MapEnts* GenerateH1MapEnts(MapEnts* asset, allocator& mem)
		{
			// allocate H1 MapEnts structure
			const auto h1_asset = mem.allocate<H1::MapEnts>();

			h1_asset->name = asset->name;

			if (ZoneTool::currentlinkermode == ZoneTool::linker_mode::iw5)
			{
				const auto str = mapents::converter::iw5::convert_mapents_ids(
					std::string{ asset->entityString, static_cast<size_t>(asset->numEntityChars) });
				h1_asset->entityString = const_cast<char*>(mem.duplicate_string(str));
				h1_asset->numEntityChars = static_cast<int>(str.size());
			}
			else
			{
				h1_asset->entityString = asset->entityString;
				h1_asset->numEntityChars = asset->numEntityChars;
			}

			h1_asset->trigger.count = asset->trigger.count;
			h1_asset->trigger.models = reinterpret_cast<H1::TriggerModel*>(asset->trigger.models);
			h1_asset->trigger.hullCount = asset->trigger.hullCount;
			h1_asset->trigger.hulls = reinterpret_cast<H1::TriggerHull*>(asset->trigger.hulls);
			h1_asset->trigger.slabCount = asset->trigger.slabCount;
			h1_asset->trigger.slabs = reinterpret_cast<H1::TriggerSlab*>(asset->trigger.slabs);

			h1_asset->clientTrigger.trigger.count = asset->clientTrigger.trigger.count;
			h1_asset->clientTrigger.trigger.models = reinterpret_cast<H1::TriggerModel*>(asset->clientTrigger.trigger.models);
			h1_asset->clientTrigger.trigger.hullCount = asset->clientTrigger.trigger.hullCount;
			h1_asset->clientTrigger.trigger.hulls = reinterpret_cast<H1::TriggerHull*>(asset->clientTrigger.trigger.hulls);
			h1_asset->clientTrigger.trigger.slabCount = asset->clientTrigger.trigger.slabCount;
			h1_asset->clientTrigger.trigger.slabs = reinterpret_cast<H1::TriggerSlab*>(asset->clientTrigger.trigger.slabs);
			h1_asset->clientTrigger.numClientTriggerNodes = asset->clientTrigger.numClientTriggerNodes;
			h1_asset->clientTrigger.clientTriggerAabbTree = reinterpret_cast<H1::ClientTriggerAabbNode*>(asset->clientTrigger.clientTriggerAabbTree);
			h1_asset->clientTrigger.triggerStringLength = asset->clientTrigger.triggerStringLength;
			h1_asset->clientTrigger.triggerString = asset->clientTrigger.triggerString;

			auto allocate = [&](const short default_value = -1)
			{
				const auto count = asset->clientTrigger.trigger.count;
				auto memory = mem.allocate<short>(asset->clientTrigger.trigger.count);
				std::fill(memory, memory + count, default_value);
				return memory;
			};

			h1_asset->clientTrigger.triggerType = allocate(0);
			h1_asset->clientTrigger.visionSetTriggers = allocate();
			h1_asset->clientTrigger.lightSetTriggers = allocate();

			for (auto i = 0; i < asset->clientTrigger.trigger.count; i++)
			{
				if ((asset->clientTrigger.triggerType[i] & CLIENT_TRIGGER_VISIONSET) != 0)
				{
					h1_asset->clientTrigger.triggerType[i] |= H1::CLIENT_TRIGGER_VISIONSET;

					h1_asset->clientTrigger.visionSetTriggers[i] = asset->clientTrigger.triggerStringOffsets[i];
					h1_asset->clientTrigger.lightSetTriggers[i] = asset->clientTrigger.triggerStringOffsets[i];
				}
			}

			h1_asset->clientTrigger.unk2 = allocate(); // todo?
			h1_asset->clientTrigger.origins = reinterpret_cast<float(*__ptr64)[3]>(asset->clientTrigger.origins);
			h1_asset->clientTrigger.scriptDelay = asset->clientTrigger.scriptDelay;
			h1_asset->clientTrigger.audioTriggers = asset->clientTrigger.audioTriggers;
			h1_asset->clientTrigger.blendLookup = allocate(); // todo?
			h1_asset->clientTrigger.unk3 = allocate(); // todo?
			h1_asset->clientTrigger.unk4 = allocate(); // todo?
			h1_asset->clientTrigger.unk5 = allocate(); // todo?
			h1_asset->clientTrigger.unk6 = allocate(); // todo?

			h1_asset->clientTriggerBlend.numClientTriggerBlendNodes = 0;
			h1_asset->clientTriggerBlend.blendNodes = nullptr;

			// generate spawnents with mapents2spawns
			h1_asset->spawnList.spawnsCount = 0;
			h1_asset->spawnList.spawns = nullptr;

			h1_asset->splineList.splineCount = 0;
			h1_asset->splineList.splines = nullptr;

			return h1_asset;
		}

		H1::MapEnts* convert(MapEnts* asset, allocator& allocator)
		{
			// generate h1 mapents
			return GenerateH1MapEnts(asset, allocator);
		}
	}
}