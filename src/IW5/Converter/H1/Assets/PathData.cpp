#include "stdafx.hpp"
#include "../Include.hpp"

#include "PathData.hpp"

namespace ZoneTool::IW5
{
	namespace H1Converter
	{
		void convert_iw5_nodetree_to_h1(H1::pathnode_tree_t* h1_tree, pathnode_tree_t* iw5_tree);
		void convert_iw5_nodetree_to_h1(H1::pathnode_tree_t* h1_tree, pathnode_tree_t* iw5_tree)
		{
			if (!iw5_tree)
			{
				return;
			}

			h1_tree->axis = iw5_tree->axis;
			h1_tree->dist = iw5_tree->dist;

			// 2 child trees
			convert_iw5_nodetree_to_h1(h1_tree->u.child[0], iw5_tree->u.child[0]);
			convert_iw5_nodetree_to_h1(h1_tree->u.child[1], iw5_tree->u.child[1]);

			h1_tree->u.s.nodeCount = iw5_tree->u.s.nodeCount;
			h1_tree->u.s.nodes = iw5_tree->u.s.nodes;
		}

		void convert_iw5_node_to_h1(H1::pathnode_t* h1_node, pathnode_t* iw5_node, int index);
		void convert_iw5_node_to_h1(H1::pathnode_t* h1_node, pathnode_t* iw5_node, int index)
		{
			if (!iw5_node)
			{
				return;
			}

#define COPY_VALUE(val) h1_node->##val = iw5_node->##val;
			COPY_VALUE(constant.type);
			COPY_VALUE(constant.spawnflags);
			COPY_VALUE(constant.targetname);
			COPY_VALUE(constant.script_linkName);
			COPY_VALUE(constant.script_noteworthy);
			COPY_VALUE(constant.target);
			COPY_VALUE(constant.animscript);
			COPY_VALUE(constant.animscriptfunc);
			h1_node->constant.vLocalOrigin[0] = iw5_node->constant.vOrigin[0];
			h1_node->constant.vLocalOrigin[1] = iw5_node->constant.vOrigin[1];
			h1_node->constant.vLocalOrigin[2] = iw5_node->constant.vOrigin[2];
			h1_node->constant.parent.name = 0;
			h1_node->constant.parent.index = 0;
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
			h1_node->constant.customangles = 0;

			h1_node->dynamic.pOwner.number = 0;
			h1_node->dynamic.pOwner.infoIndex = 0;
			COPY_VALUE(dynamic.iFreeTime);
			COPY_VALUE(dynamic.iValidTime[0]);
			COPY_VALUE(dynamic.iValidTime[1]);
			COPY_VALUE(dynamic.iValidTime[2]);
			COPY_VALUE(dynamic.wLinkCount);
			COPY_VALUE(dynamic.wOverlapCount);
			COPY_VALUE(dynamic.turretEntNumber);
			COPY_VALUE(dynamic.userCount);
			COPY_VALUE(dynamic.hasBadPlaceLink);
			h1_node->dynamic.spreadUsedTime[0] = 0;
			h1_node->dynamic.spreadUsedTime[1] = 0;
			h1_node->dynamic.flags = 0; // oh gosh
			h1_node->dynamic.dangerousCount = 0;
			h1_node->dynamic.recentUseProxTime = 0;

			COPY_VALUE(transient.iSearchFrame);
			convert_iw5_node_to_h1(h1_node->transient.pNextOpen, iw5_node->transient.pNextOpen, index);
			convert_iw5_node_to_h1(h1_node->transient.pPrevOpen, iw5_node->transient.pPrevOpen, index);
			convert_iw5_node_to_h1(h1_node->transient.pParent, iw5_node->transient.pParent, index);
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
				convert_iw5_node_to_h1(&h1_asset->nodes[i], &asset->nodes[i], i);
			}

			h1_asset->parentIndexResolved = false;
			h1_asset->version = 26;

#define COPY_VALUE(val) h1_asset->##val = asset->##val;

			COPY_VALUE(visBytes);
			memcpy(h1_asset->pathVis, asset->pathVis, sizeof(char*));
			COPY_VALUE(nodeTreeCount);

			convert_iw5_nodetree_to_h1(h1_asset->nodeTree, asset->nodeTree);

			h1_asset->dynamicNodeGroupCount = 0;
			h1_asset->dynamicNodeGroups = nullptr;
			h1_asset->exposureBytes = 0;
			h1_asset->pathExposure = nullptr;
			h1_asset->noPeekVisBytes = 0;
			h1_asset->pathNoPeekVis = nullptr;
			h1_asset->zoneCount = 0;
			h1_asset->zonesBytes = 0;
			h1_asset->pathZones = nullptr;
			h1_asset->dynStatesBytes = 0;
			h1_asset->pathDynStates = nullptr;

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
