#include "stdafx.hpp"

namespace ZoneTool::H1
{
	void IPhysCollmap::dump(PhysCollmap* asset)
	{
		const auto path = "physcollmap\\"s + asset->name + ".pc";

		assetmanager::dumper write;
		if (!write.open(path))
		{
			return;
		}

		write.dump_single(asset);
		write.dump_string(asset->name);
		write.dump_array(asset->geoms, asset->count);

		for (auto i = 0u; i < asset->count; i++)
		{
			const auto data = asset->geoms[i].data;
			write.dump_single(data);
			if (data)
			{
				write.dump_array(data->m_aVertices, data->m_vertexCount);
				write.dump_array(data->m_aPlanes, data->m_faceCount);
				write.dump_array(data->m_surfaceTypes, data->m_faceCount);
				write.dump_array(data->m_vertexMaterials, data->m_vertexCount);
				write.dump_array(data->m_aSubEdges, data->m_subEdgeCount);
				write.dump_array(data->m_aFaceSubEdges, data->m_faceCount);
			}
		}
	}
}
