#include "stdafx.hpp"
#include "../Include.hpp"

#include "ClipMap.hpp"

namespace ZoneTool::IW5
{
	namespace IW6Converter
	{
		namespace
		{
			IW6::CSurfaceFlags surf_flags_conversion_table[31]
			{
				IW6::SURF_FLAG_DEFAULT,
				IW6::SURF_FLAG_BARK,
				IW6::SURF_FLAG_BRICK,
				IW6::SURF_FLAG_CARPET,
				IW6::SURF_FLAG_CLOTH,
				IW6::SURF_FLAG_CONCRETE,
				IW6::SURF_FLAG_DIRT,
				IW6::SURF_FLAG_FLESH,
				IW6::SURF_FLAG_FOLIAGE,
				IW6::SURF_FLAG_GLASS,
				IW6::SURF_FLAG_GRASS,
				IW6::SURF_FLAG_GRAVEL,
				IW6::SURF_FLAG_ICE,
				IW6::SURF_FLAG_METAL,
				IW6::SURF_FLAG_MUD,
				IW6::SURF_FLAG_PAPER,
				IW6::SURF_FLAG_PLASTER,
				IW6::SURF_FLAG_ROCK,
				IW6::SURF_FLAG_SAND,
				IW6::SURF_FLAG_SNOW,
				IW6::SURF_FLAG_WATER,
				IW6::SURF_FLAG_WOOD,
				IW6::SURF_FLAG_ASPHALT,
				IW6::SURF_FLAG_CERAMIC,
				IW6::SURF_FLAG_PLASTIC,
				IW6::SURF_FLAG_RUBBER,
				IW6::SURF_FLAG_CUSHION,
				IW6::SURF_FLAG_FRUIT,
				IW6::SURF_FLAG_PAINTEDMETAL,
				IW6::SURF_FLAG_RIOTSHIELD,
				IW6::SURF_FLAG_SLUSH,
			}; IW5::CSurfaceFlags;

			int convert_surf_flags(int flags)
			{
				int iw6_flags = surf_flags_conversion_table[flags >> 20];
				auto convert = [&](IW5::CSurfaceFlags a, IW6::CSurfaceFlags b)
				{
					iw6_flags |= ((flags & a) == a) ? b : 0;
				};
				convert(IW5::CSurfaceFlags::SURF_FLAG_OPAQUEGLASS, IW6::CSurfaceFlags::SURF_FLAG_OPAQUEGLASS);
				convert(IW5::CSurfaceFlags::SURF_FLAG_CLIPMISSILE, IW6::CSurfaceFlags::SURF_FLAG_CLIPMISSILE);
				convert(IW5::CSurfaceFlags::SURF_FLAG_AI_NOSIGHT, IW6::CSurfaceFlags::SURF_FLAG_AI_NOSIGHT);
				convert(IW5::CSurfaceFlags::SURF_FLAG_CLIPSHOT, IW6::CSurfaceFlags::SURF_FLAG_CLIPSHOT);
				convert(IW5::CSurfaceFlags::SURF_FLAG_PLAYERCLIP, IW6::CSurfaceFlags::SURF_FLAG_PLAYERCLIP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_MONSTERCLIP, IW6::CSurfaceFlags::SURF_FLAG_MONSTERCLIP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_AICLIPALLOWDEATH, IW6::CSurfaceFlags::SURF_FLAG_AICLIPALLOWDEATH);
				convert(IW5::CSurfaceFlags::SURF_FLAG_VEHICLECLIP, IW6::CSurfaceFlags::SURF_FLAG_VEHICLECLIP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_ITEMCLIP, IW6::CSurfaceFlags::SURF_FLAG_ITEMCLIP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NODROP, IW6::CSurfaceFlags::SURF_FLAG_NODROP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NONSOLID, IW6::CSurfaceFlags::SURF_FLAG_NONSOLID);
				convert(IW5::CSurfaceFlags::SURF_FLAG_DETAIL, IW6::CSurfaceFlags::SURF_FLAG_DETAIL);
				convert(IW5::CSurfaceFlags::SURF_FLAG_STRUCTURAL, IW6::CSurfaceFlags::SURF_FLAG_STRUCTURAL);
				convert(IW5::CSurfaceFlags::SURF_FLAG_PORTAL, IW6::CSurfaceFlags::SURF_FLAG_PORTAL);
				convert(IW5::CSurfaceFlags::SURF_FLAG_CANSHOOTCLIP, IW6::CSurfaceFlags::SURF_FLAG_CANSHOOTCLIP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_ORIGIN, IW6::CSurfaceFlags::SURF_FLAG_ORIGIN);
				convert(IW5::CSurfaceFlags::SURF_FLAG_SKY, IW6::CSurfaceFlags::SURF_FLAG_SKY);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOCASTSHADOW, IW6::CSurfaceFlags::SURF_FLAG_NOCASTSHADOW);
				convert(IW5::CSurfaceFlags::SURF_FLAG_PHYSICSGEOM, IW6::CSurfaceFlags::SURF_FLAG_PHYSICSGEOM);
				convert(IW5::CSurfaceFlags::SURF_FLAG_LIGHTPORTAL, IW6::CSurfaceFlags::SURF_FLAG_LIGHTPORTAL);
				convert(IW5::CSurfaceFlags::SURF_FLAG_OUTDOORBOUNDS, IW6::CSurfaceFlags::SURF_FLAG_OUTDOORBOUNDS);
				convert(IW5::CSurfaceFlags::SURF_FLAG_SLICK, IW6::CSurfaceFlags::SURF_FLAG_SLICK);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOIMPACT, IW6::CSurfaceFlags::SURF_FLAG_NOIMPACT);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOMARKS, IW6::CSurfaceFlags::SURF_FLAG_NOMARKS);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOPENETRATE, IW6::CSurfaceFlags::SURF_FLAG_NOPENETRATE);
				convert(IW5::CSurfaceFlags::SURF_FLAG_LADDER, IW6::CSurfaceFlags::SURF_FLAG_LADDER);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NODAMAGE, IW6::CSurfaceFlags::SURF_FLAG_NODAMAGE);
				convert(IW5::CSurfaceFlags::SURF_FLAG_MANTLEON, IW6::CSurfaceFlags::SURF_FLAG_MANTLEON);
				convert(IW5::CSurfaceFlags::SURF_FLAG_MANTLEOVER, IW6::CSurfaceFlags::SURF_FLAG_MANTLEOVER);
				convert(IW5::CSurfaceFlags::SURF_FLAG_STAIRS, IW6::CSurfaceFlags::SURF_FLAG_STAIRS);
				convert(IW5::CSurfaceFlags::SURF_FLAG_SOFT, IW6::CSurfaceFlags::SURF_FLAG_SOFT);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOSTEPS, IW6::CSurfaceFlags::SURF_FLAG_NOSTEPS);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NODRAW, IW6::CSurfaceFlags::SURF_FLAG_NODRAW);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOLIGHTMAP, IW6::CSurfaceFlags::SURF_FLAG_NOLIGHTMAP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NODLIGHT, IW6::CSurfaceFlags::SURF_FLAG_NODLIGHT);
				return iw6_flags;
			}
		}

		void GenerateIW6ClipInfo(IW6::ClipInfo* info, IW5::ClipInfo* dinfo, ZoneMemory* mem)
		{
			if (!dinfo)
			{
				info = nullptr;
				return;
			}

			info->planeCount = dinfo->numCPlanes;
			info->planes = reinterpret_cast<IW6::cplane_s*>(dinfo->cPlanes);

			info->numMaterials = dinfo->numMaterials;
			info->materials = mem->Alloc<IW6::ClipMaterial>(info->numMaterials);
			for (unsigned int i = 0; i < info->numMaterials; i++)
			{
				info->materials[i].name = dinfo->materials[i].material;
				info->materials[i].surfaceFlags = convert_surf_flags(dinfo->materials[i].surfaceFlags);
				info->materials[i].contents = dinfo->materials[i].contentFlags;
			}

			std::unordered_map<cbrushside_t*, IW6::cbrushside_t*> mapped_brush_sides;

			info->numBrushSides = dinfo->numCBrushSides;
			info->brushsides = mem->Alloc<IW6::cbrushside_t>(info->numBrushSides);
			for (unsigned int i = 0; i < info->numBrushSides; i++)
			{
				mapped_brush_sides[&dinfo->cBrushSides[i]] = &info->brushsides[i];

				info->brushsides[i].plane = reinterpret_cast<IW6::cplane_s*>(dinfo->cBrushSides[i].plane);
				info->brushsides[i].materialNum = dinfo->cBrushSides[i].materialNum;
				info->brushsides[i].firstAdjacentSideOffset = dinfo->cBrushSides[i].firstAdjacentSideOffset;
				info->brushsides[i].edgeCount = dinfo->cBrushSides[i].edgeCount;
			}

			info->numBrushEdges = dinfo->numCBrushEdges;
			info->brushEdges = reinterpret_cast<IW6::cbrushedge_t*>(dinfo->cBrushEdges);

			info->leafbrushNodesCount = dinfo->numCLeafBrushNodes;
			info->leafbrushNodes = mem->Alloc<IW6::cLeafBrushNode_s>(info->leafbrushNodesCount);
			for (unsigned int i = 0; i < info->leafbrushNodesCount; i++)
			{
				info->leafbrushNodes[i].axis = dinfo->cLeafBrushNodes[i].axis;
				info->leafbrushNodes[i].leafBrushCount = dinfo->cLeafBrushNodes[i].leafBrushCount;
				info->leafbrushNodes[i].contents = dinfo->cLeafBrushNodes[i].contents;
				if (info->leafbrushNodes[i].leafBrushCount > 0)
				{
					info->leafbrushNodes[i].data.leaf.brushes = reinterpret_cast<unsigned __int16*>(
						dinfo->cLeafBrushNodes[i].data.leaf.brushes);
				}
				else
				{
					memcpy(&info->leafbrushNodes[i].data.children, &dinfo->cLeafBrushNodes[i].data.children,
						sizeof(dinfo->cLeafBrushNodes[i].data.children));
				}
			}

			info->numLeafBrushes = dinfo->numLeafBrushes;
			info->leafbrushes = reinterpret_cast<unsigned __int16*>(dinfo->leafBrushes);

			info->numBrushes = dinfo->numBrushes;
			info->brushes = mem->Alloc<IW6::cbrush_t>(info->numBrushes);
			for (unsigned int i = 0; i < info->numBrushes; i++)
			{
				info->brushes[i].numsides = dinfo->brushes[i].numsides;
				info->brushes[i].glassPieceIndex = dinfo->brushes[i].glassPieceIndex;

				if (dinfo->brushes[i].sides)
					info->brushes[i].sides = mapped_brush_sides.find(dinfo->brushes[i].sides)->second;

				//info->brushes[i].sides = reinterpret_cast<IW6::cbrushside_t*>(dinfo->brushes[i].sides);
				info->brushes[i].baseAdjacentSide = reinterpret_cast<IW6::cbrushedge_t*>(dinfo->brushes[i].edge);

				info->brushes[i].axialMaterialNum[0][0] = dinfo->brushes[i].axialMaterialNum[0][0];
				info->brushes[i].axialMaterialNum[0][1] = dinfo->brushes[i].axialMaterialNum[0][1];
				info->brushes[i].axialMaterialNum[0][2] = dinfo->brushes[i].axialMaterialNum[0][2];
				info->brushes[i].axialMaterialNum[1][0] = dinfo->brushes[i].axialMaterialNum[1][0];
				info->brushes[i].axialMaterialNum[1][1] = dinfo->brushes[i].axialMaterialNum[1][1];
				info->brushes[i].axialMaterialNum[1][2] = dinfo->brushes[i].axialMaterialNum[1][2];

				info->brushes[i].firstAdjacentSideOffsets[0][0] = dinfo->brushes[i].firstAdjacentSideOffsets[0][0];
				info->brushes[i].firstAdjacentSideOffsets[0][1] = dinfo->brushes[i].firstAdjacentSideOffsets[0][1];
				info->brushes[i].firstAdjacentSideOffsets[0][2] = dinfo->brushes[i].firstAdjacentSideOffsets[0][2];
				info->brushes[i].firstAdjacentSideOffsets[1][0] = dinfo->brushes[i].firstAdjacentSideOffsets[1][0];
				info->brushes[i].firstAdjacentSideOffsets[1][1] = dinfo->brushes[i].firstAdjacentSideOffsets[1][1];
				info->brushes[i].firstAdjacentSideOffsets[1][2] = dinfo->brushes[i].firstAdjacentSideOffsets[1][2];

				info->brushes[i].edgeCount[0][0] = dinfo->brushes[i].edgeCount[0][0];
				info->brushes[i].edgeCount[0][1] = dinfo->brushes[i].edgeCount[0][1];
				info->brushes[i].edgeCount[0][2] = dinfo->brushes[i].edgeCount[0][2];
				info->brushes[i].edgeCount[1][0] = dinfo->brushes[i].edgeCount[1][0];
				info->brushes[i].edgeCount[1][1] = dinfo->brushes[i].edgeCount[1][1];
				info->brushes[i].edgeCount[1][2] = dinfo->brushes[i].edgeCount[1][2];
			}

			info->brushBounds = reinterpret_cast<IW6::Bounds*>(dinfo->brushBounds);
			info->brushContents = reinterpret_cast<int*>(dinfo->brushContents);
		}

		IW6::clipMap_t* GenerateIW6ClipMap(clipMap_t* asset, ZoneMemory* mem)
		{
			// allocate IW6 clipMap_t structure
			const auto iw6_asset = mem->Alloc<IW6::clipMap_t>();

			iw6_asset->name = asset->name;
			iw6_asset->isInUse = asset->isInUse;
			GenerateIW6ClipInfo(&iw6_asset->info, &asset->info, mem);
			iw6_asset->pInfo = &iw6_asset->info;
			iw6_asset->numStaticModels = asset->numStaticModels;
			iw6_asset->staticModelList = mem->Alloc<IW6::cStaticModel_s>(iw6_asset->numStaticModels);
			for (unsigned int i = 0; i < iw6_asset->numStaticModels; i++)
			{
				iw6_asset->staticModelList[i].xmodel = reinterpret_cast<IW6::XModel*>(asset->staticModelList[i].xmodel);
				memcpy(&iw6_asset->staticModelList[i].origin, &asset->staticModelList[i].origin, sizeof(IW5::cStaticModel_s) - sizeof(IW5::XModel*));
			}
			iw6_asset->numNodes = asset->numCNodes;
			iw6_asset->nodes = mem->Alloc<IW6::cNode_t>(iw6_asset->numNodes);
			for (unsigned int i = 0; i < iw6_asset->numNodes; i++)
			{
				iw6_asset->nodes[i].plane = reinterpret_cast<IW6::cplane_s*>(asset->cNodes[i].plane);
				iw6_asset->nodes[i].children[0] = asset->cNodes[i].children[0];
				iw6_asset->nodes[i].children[1] = asset->cNodes[i].children[1];
			}
			iw6_asset->numLeafs = asset->numCLeaf;
			iw6_asset->leafs = reinterpret_cast<IW6::cLeaf_t *>(asset->cLeaf);
			iw6_asset->vertCount = asset->numVerts;
			iw6_asset->verts = reinterpret_cast<IW6::vec3_t*>(asset->verts);
			iw6_asset->triCount = asset->numTriIndices;
			iw6_asset->triIndices = reinterpret_cast<unsigned __int16*>(asset->triIndices);
			iw6_asset->triEdgeIsWalkable = reinterpret_cast<unsigned __int8*>(asset->triEdgeIsWalkable);
			iw6_asset->borderCount = asset->numCollisionBorders;
			iw6_asset->borders = reinterpret_cast<IW6::CollisionBorder *>(asset->collisionBorders);
			iw6_asset->partitionCount = asset->numCollisionPartitions;
			iw6_asset->partitions = mem->Alloc<IW6::CollisionPartition>(iw6_asset->partitionCount);
			for (int i = 0; i < iw6_asset->partitionCount; i++)
			{
				iw6_asset->partitions[i].triCount = asset->collisionPartitions[i].triCount;
				iw6_asset->partitions[i].borderCount = asset->collisionPartitions[i].borderCount;
				iw6_asset->partitions[i].firstVertSegment = 0;
				iw6_asset->partitions[i].firstTri = asset->collisionPartitions[i].firstTri;
				iw6_asset->partitions[i].borders = reinterpret_cast<IW6::CollisionBorder*>(asset->collisionPartitions[i].borders);
			}
			iw6_asset->aabbTreeCount = asset->numCollisionAABBTrees;
			iw6_asset->aabbTrees = reinterpret_cast<IW6::CollisionAabbTree*>(asset->collisionAABBTrees);
			iw6_asset->numSubModels = asset->numCModels;
			iw6_asset->cmodels = mem->Alloc<IW6::cmodel_t>(iw6_asset->numSubModels);
			for (unsigned int i = 0; i < iw6_asset->numSubModels; i++)
			{
				memcpy(&iw6_asset->cmodels[i].bounds, &asset->cModels[i].bounds, sizeof(IW5::Bounds) + sizeof(float));
				memcpy(&iw6_asset->cmodels[i].leaf, &asset->cModels[i].leaf, sizeof(IW5::cLeaf_t));

				iw6_asset->cmodels[i].info = nullptr; // mapped in h1
			}

			iw6_asset->mapEnts = mem->Alloc<IW6::MapEnts>();
			iw6_asset->mapEnts->name = asset->mapEnts->name;

			iw6_asset->stageCount = asset->stageCount;
			iw6_asset->stages = mem->Alloc<IW6::Stage>(iw6_asset->stageCount);
			for (unsigned int i = 0; i < iw6_asset->stageCount; i++)
			{
				iw6_asset->stages[i].name = reinterpret_cast<const char*>(asset->stages[i].name);
				memcpy(&iw6_asset->stages[i].origin, &asset->stages[i].origin, sizeof(float[3]));
				iw6_asset->stages[i].triggerIndex = asset->stages[i].triggerIndex;
				iw6_asset->stages[i].sunPrimaryLightIndex = asset->stages[i].sunPrimaryLightIndex;
			}
			iw6_asset->stageTrigger.count = 0;
			iw6_asset->stageTrigger.models = nullptr;
			iw6_asset->stageTrigger.hullCount = 0;
			iw6_asset->stageTrigger.hulls = nullptr;
			iw6_asset->stageTrigger.slabCount = 0;
			iw6_asset->stageTrigger.slabs = nullptr;

			iw6_asset->smodelNodeCount = asset->smodelNodeCount;
			iw6_asset->smodelNodes = reinterpret_cast<IW6::SModelAabbNode*>(asset->smodelNodes);
			for (unsigned char i = 0; i < 2; i++)
			{
				iw6_asset->dynEntCount[i] = asset->dynEntCount[i];
				iw6_asset->dynEntDefList[i] = mem->Alloc<IW6::DynEntityDef>(iw6_asset->dynEntCount[i]);
				iw6_asset->dynEntPoseList[i] = reinterpret_cast<IW6::DynEntityPose*>(asset->dynEntPoseList[i]);
				iw6_asset->dynEntClientList[i] = mem->Alloc<IW6::DynEntityClient>(iw6_asset->dynEntCount[i]);
				iw6_asset->dynEntCollList[i] = reinterpret_cast<IW6::DynEntityColl*>(asset->dynEntCollList[i]);
				for (unsigned short j = 0; j < iw6_asset->dynEntCount[i]; j++)
				{
					iw6_asset->dynEntDefList[i][j].type = static_cast<IW6::DynEntityType>(asset->dynEntDefList[i][j].type);
					memcpy(&iw6_asset->dynEntDefList[i][j].pose, &asset->dynEntDefList[i][j].pose, sizeof(IW5::GfxPlacement));
					iw6_asset->dynEntDefList[i][j].baseModel = reinterpret_cast<IW6::XModel*>(asset->dynEntDefList[i][j].xModel);
					iw6_asset->dynEntDefList[i][j].brushModel = asset->dynEntDefList[i][j].brushModel;
					iw6_asset->dynEntDefList[i][j].physicsBrushModel = asset->dynEntDefList[i][j].physicsBrushModel;
					iw6_asset->dynEntDefList[i][j].health = asset->dynEntDefList[i][j].health;
					iw6_asset->dynEntDefList[i][j].destroyFx = reinterpret_cast<IW6::FxEffectDef*>(asset->dynEntDefList[i][j].destroyFx);
					iw6_asset->dynEntDefList[i][j].physPreset = reinterpret_cast<IW6::PhysPreset*>(asset->dynEntDefList[i][j].physPreset);
					iw6_asset->dynEntDefList[i][j].hinge = reinterpret_cast<IW6::DynEntityHingeDef*>(asset->dynEntDefList[i][j].hinge);
					iw6_asset->dynEntDefList[i][j].linkTo = nullptr;
					memcpy(&iw6_asset->dynEntDefList[i][j].mass, &asset->dynEntDefList[i][j].mass, sizeof(IW5::PhysMass) + sizeof(int));

					iw6_asset->dynEntClientList[i][j].physObjId = asset->dynEntClientList[i]->physObjId;
					iw6_asset->dynEntClientList[i][j].flags = asset->dynEntClientList[i]->flags;
					iw6_asset->dynEntClientList[i][j].lightingHandle = asset->dynEntClientList[i]->lightingHandle;
					iw6_asset->dynEntClientList[i][j].health = asset->dynEntClientList[i]->health;
					iw6_asset->dynEntClientList[i][j].hinge = nullptr;
					iw6_asset->dynEntClientList[i][j].activeModel = nullptr;
					iw6_asset->dynEntClientList[i][j].contents = asset->dynEntClientList[i]->contents;
				}
			}
			iw6_asset->checksum = asset->isPlutoniumMap;

			return iw6_asset;
		}

		IW6::clipMap_t* convert(clipMap_t* asset, ZoneMemory* mem)
		{
			return GenerateIW6ClipMap(asset, mem);
		}
	}
}