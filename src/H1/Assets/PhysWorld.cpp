#include "stdafx.hpp"

namespace ZoneTool
{
	namespace H1
	{
		void IPhysWorld::dump(PhysWorld* asset, const std::function<const char* (std::uint16_t)>& SL_ConvertToString)
		{
			const auto path = asset->name + ".physmap"s;

			assetmanager::dumper write;
			if (!write.open(path))
			{
				return;
			}

			write.dump_single(asset);
			write.dump_string(asset->name);

			write.dump_array(asset->models, asset->modelsCount);
			write.dump_array(asset->polytopeDatas, asset->polytopeDatasCount);
			for (unsigned int i = 0; i < asset->polytopeDatasCount; i++)
			{
				write.dump_array(asset->polytopeDatas[i].vec4_array0, asset->polytopeDatas[i].count0);
				write.dump_array(asset->polytopeDatas[i].vec4_array1, asset->polytopeDatas[i].count1);
				write.dump_array(asset->polytopeDatas[i].uint16_array0, asset->polytopeDatas[i].count1);
				write.dump_array(asset->polytopeDatas[i].uint16_array1, asset->polytopeDatas[i].count0);
				write.dump_array(asset->polytopeDatas[i].edges, asset->polytopeDatas[i].count2);
				write.dump_array(asset->polytopeDatas[i].uint8_array0, asset->polytopeDatas[i].count1);
			}
			write.dump_array(asset->meshDatas, asset->meshDatasCount);
			for (unsigned int i = 0; i < asset->meshDatasCount; i++)
			{
				write.dump_array(asset->meshDatas[i].meshNodes, asset->meshDatas[i].count0);
				write.dump_array(asset->meshDatas[i].vec4_array0, asset->meshDatas[i].count1);
				write.dump_array(asset->meshDatas[i].meshTriangles, asset->meshDatas[i].count2);
			}
			write.dump_array(asset->waterVolumes, asset->waterVolumesCount);
			for (unsigned int i = 0; i < asset->waterVolumesCount; i++)
			{
				write.dump_asset(asset->waterVolumes[i].physWaterPreset);
				write.dump_string(SL_ConvertToString(asset->waterVolumes[i].string));
			}

			write.close();
		}
	}
}