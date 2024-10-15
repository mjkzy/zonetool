#include "stdafx.hpp"

namespace ZoneTool::H1
{
	void IPathData::dump(clipMap_t* asset, const std::function<const char* (std::uint16_t)>& SL_ConvertToString)
	{
		const auto path = asset->name + ".colmap"s;

		assetmanager::dumper write;
		if (!write.open(path))
		{
			return;
		}

		write.dump_single(asset);
		write.dump_string(asset->name);

		write.dump_array(asset->nodes, asset->numNodes);
		for (unsigned int i = 0; i < asset->numNodes; i++)
		{
			if (asset->nodes[i].plane)
			{
				write.dump_array(asset->nodes[i].plane, 1);
			}
		}

		write.dump_array(asset->leafs, asset->numLeafs);
		write.dump_array(asset->cmodels, asset->numSubModels);

		write.dump_asset(asset->mapEnts);

		write.dump_array(asset->stages, asset->stageCount);
		for (unsigned char i = 0; i < asset->stageCount; i++)
		{
			write.dump_string(asset->stages[i].name);
		}

		for (int i = 0; i < 2; i++)
		{
			write.dump_int(asset->dynEntCount[i]);

			if (asset->dynEntCount[i] <= 0)
			{
				continue;
			}

			write.dump_array(asset->dynEntDefList[i], asset->dynEntCount[i]);
			write.dump_array(asset->dynEntPoseList[i], asset->dynEntCount[i]);
			write.dump_array(asset->dynEntClientList[i], asset->dynEntCount[i]);
			write.dump_array(asset->dynEntCollList[i], asset->dynEntCount[i]);
			for (int j = 0; j < asset->dynEntCount[i]; j++)
			{
				write.dump_asset(asset->dynEntDefList[i][j].baseModel);
				write.dump_asset(asset->dynEntDefList[i][j].destroyFx);
				write.dump_asset(asset->dynEntDefList[i][j].sound);
				write.dump_asset(asset->dynEntDefList[i][j].physPreset);
				write.dump_single(asset->dynEntDefList[i][j].hinge);
				write.dump_single(asset->dynEntDefList[i][j].linkTo);

				//write.dump_single(asset->dynEntClientList[i][j].hinge);
				//write.dump_asset(asset->dynEntClientList[i][j].activeModel);
			}
		}

		for (unsigned int i = 0; i < asset->dynEntAnchorCount; i++)
		{
			write.dump_string(SL_ConvertToString(asset->dynEntAnchorNames[i]));
		}

		write.dump_array(asset->scriptableMapEnts.instances, asset->scriptableMapEnts.instanceCount);
		if (asset->scriptableMapEnts.instances)
		{
			for (unsigned int i = 0; i < asset->scriptableMapEnts.instanceCount; i++)
			{
				if (asset->scriptableMapEnts.instances[i].def)
				{
					write.dump_asset(asset->scriptableMapEnts.instances[i].def);
				}
				if (asset->scriptableMapEnts.instances[i].eventConstantsBuf && asset->scriptableMapEnts.instances[i].def)
				{
					write.dump_array(asset->scriptableMapEnts.instances[i].eventConstantsBuf,
						asset->scriptableMapEnts.instances[i].def->eventConstantsSize);
				}
				if (asset->scriptableMapEnts.instances[i].targetData)
				{
					write.dump_array(asset->scriptableMapEnts.instances[i].targetData,
						asset->scriptableMapEnts.instances[i].targetCount);
				}
				auto str = SL_ConvertToString(asset->scriptableMapEnts.instances[i].targetname);
				write.dump_string(str ? str : "");
				if (asset->scriptableMapEnts.instances[i].currentModel)
				{
					write.dump_asset(asset->scriptableMapEnts.instances[i].currentModel);
				}
				if (asset->scriptableMapEnts.instances[i].partStates && asset->scriptableMapEnts.instances[i].def)
				{
					write.dump_array(asset->scriptableMapEnts.instances[i].partStates,
						asset->scriptableMapEnts.instances[i].def->partCount);
				}
				if (asset->scriptableMapEnts.instances[i].eventStreamBuf && asset->scriptableMapEnts.instances[i].def)
				{
					write.dump_array(asset->scriptableMapEnts.instances[i].eventStreamBuf,
						asset->scriptableMapEnts.instances[i].def->eventStreamSize);
				}
			}
		}

		write.dump_array(asset->scriptableMapEnts.animEntries, asset->scriptableMapEnts.animEntryCount);
		if (asset->scriptableMapEnts.animEntries)
		{
			for (unsigned int i = 0; i < asset->scriptableMapEnts.animEntryCount; i++)
			{
				if (asset->scriptableMapEnts.animEntries[i].animName)
				{
					write.dump_string(asset->scriptableMapEnts.animEntries[i].animName);
				}
			}
		}

		write.dump_array(asset->grapple.magnetTree.tree, asset->grapple.magnetTree.treeCount);
		if (asset->grapple.magnetTree.tree)
		{
			for (int i = 0; i < asset->grapple.magnetTree.treeCount; i++)
			{
				write.dump_array(asset->grapple.magnetTree.tree[i].objIdx, asset->grapple.magnetTree.tree[i].numObjects);
			}
		}

		write.dump_array(asset->grapple.magnetTree.objects, asset->grapple.magnetTree.numObjects);

		write.dump_array(asset->grapple.magnets, asset->grapple.magnetCount);

		write.close();
	}
}
