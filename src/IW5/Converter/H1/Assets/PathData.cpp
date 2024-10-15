#include "stdafx.hpp"
#include "../Include.hpp"

#include "PathData.hpp"

namespace ZoneTool::IW5
{
	namespace H1Converter
	{
		void convert_iw5_node_to_h1(H1::pathnode_t* h1_node, PathData* iw5_pathdata, int index);
		void convert_iw5_node_to_h1(H1::pathnode_t* h1_node, PathData* iw5_pathdata, int index)
		{
			auto iw5_node = &iw5_pathdata->nodes[index];
			if (!iw5_node)
			{
				return;
			}

#define COPY_VALUE(val) h1_node->##val = iw5_node->##val;
			h1_node->constant.type = iw5_node->constant.type;
			h1_node->constant.spawnflags = iw5_node->constant.spawnflags;
			h1_node->constant.targetname = iw5_node->constant.targetname;
			h1_node->constant.script_linkName = iw5_node->constant.script_linkName;
			h1_node->constant.script_noteworthy = iw5_node->constant.script_noteworthy;
			h1_node->constant.target = iw5_node->constant.target;
			h1_node->constant.animscript = iw5_node->constant.animscript;
			h1_node->constant.animscriptfunc = iw5_node->constant.animscriptfunc;
			h1_node->constant.vLocalOrigin[0] = iw5_node->constant.vOrigin[0];
			h1_node->constant.vLocalOrigin[1] = iw5_node->constant.vOrigin[1];
			h1_node->constant.vLocalOrigin[2] = iw5_node->constant.vOrigin[2];
			h1_node->constant.___u9.yaw_orient.fLocalAngle = iw5_node->constant.fAngle;
			h1_node->constant.___u9.yaw_orient.localForward[0] = iw5_node->constant.forward[0];
			h1_node->constant.___u9.yaw_orient.localForward[1] = iw5_node->constant.forward[1];
			h1_node->constant.___u9.angles[0] = 0.0f;
			h1_node->constant.___u9.angles[1] = 0.0f;
			h1_node->constant.___u9.angles[2] = 0.0f;
			// fRadius??
			h1_node->constant.___u11.minUseDistSq = iw5_node->constant.minUseDistSq;
			h1_node->constant.___u11.error = static_cast<H1::PathNodeErrorCode>(iw5_node->constant.error);
			COPY_VALUE(constant.wOverlapNode[0]);
			COPY_VALUE(constant.wOverlapNode[1]);
			COPY_VALUE(constant.totalLinkCount);
			memcpy(h1_node->constant.Links, iw5_node->constant.Links, sizeof(pathlink_s));
			h1_node->constant.customangles = scr_string_t();

			COPY_VALUE(dynamic.iFreeTime);
			COPY_VALUE(dynamic.iValidTime[0]);
			COPY_VALUE(dynamic.iValidTime[1]);
			COPY_VALUE(dynamic.iValidTime[2]);
			COPY_VALUE(dynamic.wLinkCount);
			COPY_VALUE(dynamic.wOverlapCount);
			COPY_VALUE(dynamic.turretEntNumber);
			COPY_VALUE(dynamic.userCount);
			COPY_VALUE(dynamic.hasBadPlaceLink);
			//COPY_VALUE(dynamic.spreadUseTime);
			h1_node->dynamic.flags = 0;
			h1_node->dynamic.dangerousCount = 0;
			h1_node->dynamic.recentUseProxTime = 0;
			//h1_node->dynamic
			//h1_node->transient

			COPY_VALUE(transient.iSearchFrame);
			convert_iw5_node_to_h1(h1_node->transient.pNextOpen, iw5_node->transient.pNextOpen);
			convert_iw5_node_to_h1(h1_node->transient.pPrevOpen, iw5_node->transient.pPrevOpen);
			convert_iw5_node_to_h1(h1_node->transient.pParent, iw5_node->transient.pParent);
			COPY_VALUE(transient.fCost);
			COPY_VALUE(transient.fHeuristic);
			COPY_VALUE(transient.___u6.nodeCost);
			COPY_VALUE(transient.___u6.linkIndex);
		}

		H1::PathData* GenerateH1PathData(PathData* asset, allocator& mem)
		{
			// allocate H1 PathData structure
			const auto h1_asset = mem.allocate<H1::PathData>();

			h1_asset->name = asset->name;
			h1_asset->nodes = mem.allocate<H1::pathnode_t>(h1_asset->nodeCount);

			for (unsigned int i = 0; i < asset->nodeCount; ++i)
			{
				auto h1_node = h1_asset->nodes[i];
				convert_iw5_node_to_h1(&h1_node, asset, i);
			}

			h1_asset-> = mem.allocate<H1::pathnode_t>(h1_asset->nodeCount);

			/*
			h1_asset->name = asset->name;
			h1_asset->isInUse = asset->isInUse;

			GenerateH1ClipInfo(&h1_asset->info, asset, mem);
			h1_asset->pInfo = &h1_asset->info;

			h1_asset->numNodes = asset->numNodes;
			h1_asset->nodes = mem.allocate<H1::cNode_t>(h1_asset->numNodes);
			for (unsigned int i = 0; i < h1_asset->numNodes; i++)
			{
				REINTERPRET_CAST_SAFE(h1_asset->nodes[i].plane, asset->nodes[i].plane);
				h1_asset->nodes[i].children[0] = asset->nodes[i].children[0];
				h1_asset->nodes[i].children[1] = asset->nodes[i].children[1];
			}

			h1_asset->numLeafs = asset->numLeafs;
			h1_asset->leafs = mem.allocate<H1::cLeaf_t>(h1_asset->numLeafs);
			for (unsigned int i = 0; i < h1_asset->numLeafs; i++)
			{
				h1_asset->leafs[i].firstCollAabbIndex = asset->leafs[i].firstCollAabbIndex;
				h1_asset->leafs[i].collAabbCount = asset->leafs[i].collAabbCount;
				h1_asset->leafs[i].brushContents = asset->leafs[i].brushContents;
				h1_asset->leafs[i].terrainContents = asset->leafs[i].terrainContents;
				memcpy(&h1_asset->leafs[i].bounds, &asset->leafs[i].bounds, sizeof(IW5::Bounds));
				h1_asset->leafs[i].leafBrushNode = asset->leafs[i].leafBrushNode;
			}
			*/

			return h1_asset;
		}

		H1::PathData* convert(PathData* asset, allocator& allocator)
		{
			// generate h1 PathData
			return GenerateH1PathData(asset, allocator);
		}
	}
}
