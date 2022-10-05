#include "stdafx.hpp"
#include "H1/Assets/ClipMap.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		void GenerateH1ClipInfo(H1::ClipInfo* info, clipMap_t* asset, ZoneMemory* mem)
		{
			info->planeCount = asset->info.numCPlanes;
			REINTERPRET_CAST_SAFE(info->planes, asset->info.cPlanes);

			info->numMaterials = asset->info.numMaterials;
			info->materials = mem->Alloc<H1::ClipMaterial>(info->numMaterials);
			for (unsigned int i = 0; i < info->numMaterials; i++)
			{
				info->materials[i].name = asset->info.materials[i].material;
				info->materials[i].surfaceFlags = asset->info.materials[i].surfaceFlags;
				info->materials[i].contents = asset->info.materials[i].contentFlags;
			}

			info->bCollisionTree.leafbrushNodesCount = asset->info.numCLeafBrushNodes;
			info->bCollisionTree.leafbrushNodes = mem->Alloc<H1::cLeafBrushNode_s>(info->bCollisionTree.leafbrushNodesCount);
			for (unsigned int i = 0; i < info->bCollisionTree.leafbrushNodesCount; i++)
			{
				info->bCollisionTree.leafbrushNodes[i].axis = asset->info.cLeafBrushNodes[i].axis;
				info->bCollisionTree.leafbrushNodes[i].leafBrushCount = asset->info.cLeafBrushNodes[i].leafBrushCount;
				info->bCollisionTree.leafbrushNodes[i].contents = asset->info.cLeafBrushNodes[i].contents;
				if (info->bCollisionTree.leafbrushNodes[i].leafBrushCount > 0)
				{
					REINTERPRET_CAST_SAFE(info->bCollisionTree.leafbrushNodes[i].data.leaf.brushes, asset->info.cLeafBrushNodes[i].data.leaf.brushes);
				}
				else
				{
					// could be wrong
					memcpy(&info->bCollisionTree.leafbrushNodes[i].data.children, &asset->info.cLeafBrushNodes[i].data.children,
						sizeof(asset->info.cLeafBrushNodes[i].data.children));
				}
			}

			info->bCollisionTree.numLeafBrushes = asset->info.numLeafBrushes;
			REINTERPRET_CAST_SAFE(info->bCollisionTree.leafbrushes, asset->info.leafBrushes);

			info->pCollisionTree.aabbTreeCount = asset->numCollisionAABBTrees;
			REINTERPRET_CAST_SAFE(info->pCollisionTree.aabbTrees, asset->collisionAABBTrees); // origin -> midPoint

			info->sCollisionTree.unknown = 0;
			info->sCollisionTree.smodelNodeCount = asset->smodelNodeCount;
			REINTERPRET_CAST_SAFE(info->sCollisionTree.smodelNodes, asset->smodelNodes);

			std::unordered_map<cbrushside_t*, H1::cbrushside_t*> mapped_brush_sides;

			info->bCollisionData.numBrushSides = asset->info.numCBrushSides;
			info->bCollisionData.brushSides = mem->Alloc<H1::cbrushside_t>(info->bCollisionData.numBrushSides);
			for (unsigned int i = 0; i < info->bCollisionData.numBrushSides; i++)
			{
				mapped_brush_sides[&asset->info.cBrushSides[i]] = &info->bCollisionData.brushSides[i];

				// check
				info->bCollisionData.brushSides[i].planeIndex = 
					(reinterpret_cast<std::uintptr_t>(asset->info.cBrushSides[i].plane) - reinterpret_cast<std::uintptr_t>(asset->info.cPlanes)) / sizeof(cplane_s);
				info->bCollisionData.brushSides[i].materialNum = asset->info.cBrushSides[i].materialNum;
				assert(info->bCollisionData.brushSides[i].planeIndex <= info->planeCount);

				//info->bCollisionData.brushSides[i].firstAdjacentSideOffset = 0;
				//info->bCollisionData.brushSides[i].edgeCount = 0;
			}

			info->bCollisionData.numBrushEdges = asset->info.numCBrushEdges;
			REINTERPRET_CAST_SAFE(info->bCollisionData.brushEdges, asset->info.cBrushEdges);

			info->bCollisionData.numBrushes = asset->info.numBrushes;
			info->bCollisionData.brushes = mem->Alloc<H1::cbrush_t>(asset->info.numBrushes);
			for (unsigned int i = 0; i < info->bCollisionData.numBrushes; i++)
			{
				info->bCollisionData.brushes[i].numsides = asset->info.brushes[i].numsides;
				info->bCollisionData.brushes[i].glassPieceIndex = asset->info.brushes[i].glassPieceIndex;

				if (asset->info.brushes[i].sides)
					info->bCollisionData.brushes[i].sides = mapped_brush_sides.find(asset->info.brushes[i].sides)->second;

				REINTERPRET_CAST_SAFE(info->bCollisionData.brushes[i].baseAdjacentSide, asset->info.brushes[i].edge);

				info->bCollisionData.brushes[i].axialMaterialNum[0][0] = asset->info.brushes[i].axialMaterialNum[0][0];
				info->bCollisionData.brushes[i].axialMaterialNum[0][1] = asset->info.brushes[i].axialMaterialNum[0][1];
				info->bCollisionData.brushes[i].axialMaterialNum[0][2] = asset->info.brushes[i].axialMaterialNum[0][2];
				info->bCollisionData.brushes[i].axialMaterialNum[1][0] = asset->info.brushes[i].axialMaterialNum[1][0];
				info->bCollisionData.brushes[i].axialMaterialNum[1][1] = asset->info.brushes[i].axialMaterialNum[1][1];
				info->bCollisionData.brushes[i].axialMaterialNum[1][2] = asset->info.brushes[i].axialMaterialNum[1][2];

				info->bCollisionData.brushes[i].firstAdjacentSideOffsets[0][0] = asset->info.brushes[i].firstAdjacentSideOffsets[0][0];
				info->bCollisionData.brushes[i].firstAdjacentSideOffsets[0][1] = asset->info.brushes[i].firstAdjacentSideOffsets[0][1];
				info->bCollisionData.brushes[i].firstAdjacentSideOffsets[0][2] = asset->info.brushes[i].firstAdjacentSideOffsets[0][2];
				info->bCollisionData.brushes[i].firstAdjacentSideOffsets[1][0] = asset->info.brushes[i].firstAdjacentSideOffsets[1][0];
				info->bCollisionData.brushes[i].firstAdjacentSideOffsets[1][1] = asset->info.brushes[i].firstAdjacentSideOffsets[1][1];
				info->bCollisionData.brushes[i].firstAdjacentSideOffsets[1][2] = asset->info.brushes[i].firstAdjacentSideOffsets[1][2];

				info->bCollisionData.brushes[i].edgeCount[0][0] = asset->info.brushes[i].edgeCount[0][0];
				info->bCollisionData.brushes[i].edgeCount[0][1] = asset->info.brushes[i].edgeCount[0][1];
				info->bCollisionData.brushes[i].edgeCount[0][2] = asset->info.brushes[i].edgeCount[0][2];
				info->bCollisionData.brushes[i].edgeCount[1][0] = asset->info.brushes[i].edgeCount[1][0];
				info->bCollisionData.brushes[i].edgeCount[1][1] = asset->info.brushes[i].edgeCount[1][1];
				info->bCollisionData.brushes[i].edgeCount[1][2] = asset->info.brushes[i].edgeCount[1][2];
			}

			REINTERPRET_CAST_SAFE(info->bCollisionData.brushBounds, asset->info.brushBounds);
			REINTERPRET_CAST_SAFE(info->bCollisionData.brushContents, asset->info.brushContents);

			info->pCollisionData.vertCount = asset->numVerts;
			REINTERPRET_CAST_SAFE(info->pCollisionData.verts, asset->verts);
			info->pCollisionData.triCount = asset->numTriIndices;
			REINTERPRET_CAST_SAFE(info->pCollisionData.triIndices, asset->triIndices);
			REINTERPRET_CAST_SAFE(info->pCollisionData.triEdgeIsWalkable, asset->triEdgeIsWalkable);
			info->pCollisionData.borderCount = asset->numCollisionBorders;
			REINTERPRET_CAST_SAFE(info->pCollisionData.borders, asset->collisionBorders);
			info->pCollisionData.partitionCount = asset->numCollisionPartitions;
			info->pCollisionData.partitions = mem->Alloc<H1::CollisionPartition>(info->pCollisionData.partitionCount);
			for (int i = 0; i < info->pCollisionData.partitionCount; i++)
			{
				info->pCollisionData.partitions[i].triCount = asset->collisionPartitions[i].triCount;
				info->pCollisionData.partitions[i].borderCount = asset->collisionPartitions[i].borderCount;
				info->pCollisionData.partitions[i].firstVertSegment = 0;
				info->pCollisionData.partitions[i].firstTri = asset->collisionPartitions[i].firstTri;
				REINTERPRET_CAST_SAFE(info->pCollisionData.partitions[i].borders, asset->collisionPartitions[i].borders);
			}

			info->sCollisionData.numStaticModels = asset->numStaticModels;
			info->sCollisionData.staticModelList = mem->Alloc<H1::cStaticModel_s>(info->sCollisionData.numStaticModels);
			for (unsigned int i = 0; i < info->sCollisionData.numStaticModels; i++)
			{
				info->sCollisionData.staticModelList[i].xmodel = reinterpret_cast<H1::XModel * __ptr64>(asset->staticModelList[i].xmodel);

				// check
				memcpy(&info->sCollisionData.staticModelList[i].origin, &asset->staticModelList[i].origin, sizeof(IW5::cStaticModel_s) - sizeof(IW5::XModel*));
			}
		}

		H1::clipMap_t* GenerateH1ClipMap(clipMap_t* asset, ZoneMemory* mem)
		{
			// allocate H1 clipMap_t structure
			const auto h1_asset = mem->Alloc<H1::clipMap_t>();

			h1_asset->name = asset->name;
			h1_asset->isInUse = asset->isInUse;

			GenerateH1ClipInfo(&h1_asset->info, asset, mem);
			h1_asset->pInfo = &h1_asset->info;

			h1_asset->numSubModels = asset->numCModels;
			h1_asset->cmodels = mem->Alloc<H1::cmodel_t>(h1_asset->numSubModels);
			for (unsigned int i = 0; i < h1_asset->numSubModels; i++)
			{
				memcpy(&h1_asset->cmodels[i].bounds, &asset->cModels[i].bounds, sizeof(IW5::Bounds) + sizeof(float));
				//GenerateH1ClipInfo(iw6_asset->cmodels[i].info, asset->cModels[i].info, mem);
				memcpy(&h1_asset->cmodels[i].leaf, &asset->cModels[i].leaf, sizeof(IW5::cLeaf_t)); // check
				//if (!iw6_asset->cmodels[i].info)
				{
					h1_asset->cmodels[i].info = h1_asset->pInfo;
				}
			}

			h1_asset->mapEnts = mem->Alloc<H1::MapEnts>();
			h1_asset->mapEnts->name = asset->mapEnts->name; // NEED TO DO MAPENTs LATER

			h1_asset->stageCount = asset->stageCount;
			h1_asset->stages = mem->Alloc<H1::Stage>(h1_asset->stageCount);
			for (unsigned int i = 0; i < h1_asset->stageCount; i++)
			{
				h1_asset->stages[i].name = reinterpret_cast<const char* __ptr64>(asset->stages[i].name);
				memcpy(&h1_asset->stages[i].origin, &asset->stages[i].origin, sizeof(IW5::Stage) - sizeof(const char*)); // check
			}
			h1_asset->stageTrigger; // NEED TO DO THIS LATER

			for (unsigned char i = 0; i < 2; i++)
			{
				h1_asset->dynEntCount[i] = asset->dynEntCount[i];
				h1_asset->dynEntDefList[i] = mem->Alloc<H1::DynEntityDef>(h1_asset->dynEntCount[i]);
				h1_asset->dynEntPoseList[i] = reinterpret_cast<H1::DynEntityPose * __ptr64>(asset->dynEntPoseList[i]); // check
				h1_asset->dynEntClientList[i] = mem->Alloc<H1::DynEntityClient>(h1_asset->dynEntCount[i]);
				h1_asset->dynEntCollList[i] = reinterpret_cast<H1::DynEntityColl * __ptr64>(asset->dynEntCollList[i]);
				for (unsigned short j = 0; j < h1_asset->dynEntCount[i]; j++)
				{
					h1_asset->dynEntDefList[i][j].type = static_cast<H1::DynEntityType>(asset->dynEntDefList[i][j].type);
					memcpy(&h1_asset->dynEntDefList[i][j].pose, &asset->dynEntDefList[i][j].pose, sizeof(IW5::GfxPlacement));
					h1_asset->dynEntDefList[i][j].baseModel = reinterpret_cast<H1::XModel * __ptr64>(asset->dynEntDefList[i][j].xModel);
					h1_asset->dynEntDefList[i][j].brushModel = asset->dynEntDefList[i][j].brushModel;
					h1_asset->dynEntDefList[i][j].physicsBrushModel = asset->dynEntDefList[i][j].physicsBrushModel;
					h1_asset->dynEntDefList[i][j].health = asset->dynEntDefList[i][j].health;
					h1_asset->dynEntDefList[i][j].destroyFx = reinterpret_cast<H1::FxEffectDef * __ptr64>(asset->dynEntDefList[i][j].destroyFx);
					h1_asset->dynEntDefList[i][j].sound = nullptr;
					h1_asset->dynEntDefList[i][j].physPreset = reinterpret_cast<H1::PhysPreset * __ptr64>(asset->dynEntDefList[i][j].physPreset);
					h1_asset->dynEntDefList[i][j].hinge = reinterpret_cast<H1::DynEntityHingeDef * __ptr64>(asset->dynEntDefList[i][j].hinge);
					h1_asset->dynEntDefList[i][j].linkTo = nullptr;
					memcpy(&h1_asset->dynEntDefList[i][j].mass, &asset->dynEntDefList[i][j].mass, sizeof(IW5::PhysMass));
					h1_asset->dynEntDefList[i][j].contents = asset->dynEntDefList[i][j].contents; // check

					h1_asset->dynEntClientList[i][j].physObjId = asset->dynEntClientList[i]->physObjId;
					h1_asset->dynEntClientList[i][j].flags = asset->dynEntClientList[i]->flags;
					h1_asset->dynEntClientList[i][j].lightingHandle = asset->dynEntClientList[i]->lightingHandle;
					h1_asset->dynEntClientList[i][j].health = asset->dynEntClientList[i]->health;
					h1_asset->dynEntClientList[i][j].hinge = nullptr;
					h1_asset->dynEntClientList[i][j].activeModel = nullptr;
					h1_asset->dynEntClientList[i][j].contents = asset->dynEntClientList[i]->contents;
				}
			}

			h1_asset->dynEntAnchorCount = 0;
			h1_asset->dynEntAnchorNames = nullptr;

			h1_asset->scriptableMapEnts;

			h1_asset->grappleData;

			h1_asset->checksum = asset->isPlutoniumMap;

			return h1_asset;
		}

		void IClipMap::dump(clipMap_t* asset, ZoneMemory* mem)
		{
			// generate h1 clipmap
			auto h1_asset = GenerateH1ClipMap(asset, mem);

			// dump h1 clipmap
			H1::IClipMap::dump(h1_asset, SL_ConvertToString);
		}
	}
}