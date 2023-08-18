#include "stdafx.hpp"
#include "H1/Assets/MapEnts.hpp"

#include "Converter/H1/Assets/MapEnts.hpp"

namespace ZoneTool::IW5
{
	void IMapEnts::dump(MapEnts* asset, ZoneMemory* mem)
	{
		// generate h1 mapents
		auto* iw6_asset = Converter::convert(asset, mem);

		// dump h1 mapents
		H1::IMapEnts::dump(iw6_asset, SL_ConvertToString);

		// dump spawns
		mapents2spawns::dump_spawns(filesystem::get_dump_path() + asset->name + ".ents.spawnList"s, asset->entityString);
	}
}