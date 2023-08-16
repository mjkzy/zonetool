#include "stdafx.hpp"
#include "H1/Assets/XSurface.hpp"

#include "H1/Utils/Utils.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		void GenerateH1BlendVertsShit(H1::XSurface* surf)
		{
			unsigned short a = 0;
			unsigned short b = 0;
			unsigned short index = 1;
			for (short s = 0; s < (surf->blendVertCounts[0]); s++)
			{
				surf->blendVertsTable[a].b[0] = surf->blendVerts[b + 0] / 64;
				surf->blendVertsTable[a].b[1] = 0;
				surf->blendVertsTable[a].b[2] = 0;
				surf->blendVertsTable[a].b[3] = 0;
				surf->blendVertsTable[a].b[4] = 0;
				surf->blendVertsTable[a].b[5] = 0;
				surf->blendVertsTable[a].b[6] = 0;
				surf->blendVertsTable[a].b[7] = 0;

				surf->blendVertsTable[a].b[8] = 0;
				surf->blendVertsTable[a].b[9] = 0;
				surf->blendVertsTable[a].b[10] = 0;
				surf->blendVertsTable[a].b[11] = 0;
				surf->blendVertsTable[a].b[12] = 0;
				surf->blendVertsTable[a].b[13] = 0;
				surf->blendVertsTable[a].b[14] = 0;

				surf->blendVertsTable[a].blendVertCountIndex = index;

				a++;
				b += 1;
			}
			index++;

			for (short s = 0; s < (surf->blendVertCounts[1]); s++)
			{
				surf->blendVertsTable[a].b[0] = surf->blendVerts[b + 0] / 64;
				surf->blendVertsTable[a].b[1] = surf->blendVerts[b + 1] / 64;
				surf->blendVertsTable[a].b[2] = 0;
				surf->blendVertsTable[a].b[3] = 0;
				surf->blendVertsTable[a].b[4] = 0;
				surf->blendVertsTable[a].b[5] = 0;
				surf->blendVertsTable[a].b[6] = 0;
				surf->blendVertsTable[a].b[7] = 0;

				surf->blendVertsTable[a].b[8] = surf->blendVerts[b + 2];
				surf->blendVertsTable[a].b[9] = 0;
				surf->blendVertsTable[a].b[10] = 0;
				surf->blendVertsTable[a].b[11] = 0;
				surf->blendVertsTable[a].b[12] = 0;
				surf->blendVertsTable[a].b[13] = 0;
				surf->blendVertsTable[a].b[14] = 0;

				surf->blendVertsTable[a].blendVertCountIndex = index;

				a++;
				b += 3;
			}
			index++;

			for (short s = 0; s < (surf->blendVertCounts[2]); s++)
			{
				surf->blendVertsTable[a].b[0] = surf->blendVerts[b + 0] / 64;
				surf->blendVertsTable[a].b[1] = surf->blendVerts[b + 1] / 64;
				surf->blendVertsTable[a].b[2] = surf->blendVerts[b + 3] / 64;
				surf->blendVertsTable[a].b[3] = 0;
				surf->blendVertsTable[a].b[4] = 0;
				surf->blendVertsTable[a].b[5] = 0;
				surf->blendVertsTable[a].b[6] = 0;
				surf->blendVertsTable[a].b[7] = 0;

				surf->blendVertsTable[a].b[8] = surf->blendVerts[b + 2];
				surf->blendVertsTable[a].b[9] = surf->blendVerts[b + 4];
				surf->blendVertsTable[a].b[10] = 0;
				surf->blendVertsTable[a].b[11] = 0;
				surf->blendVertsTable[a].b[12] = 0;
				surf->blendVertsTable[a].b[13] = 0;
				surf->blendVertsTable[a].b[14] = 0;

				surf->blendVertsTable[a].blendVertCountIndex = index;

				a++;
				b += 5;
			}
			index++;

			for (short s = 0; s < (surf->blendVertCounts[3]); s++)
			{
				surf->blendVertsTable[a].b[0] = surf->blendVerts[b + 0] / 64;
				surf->blendVertsTable[a].b[1] = surf->blendVerts[b + 1] / 64;
				surf->blendVertsTable[a].b[2] = surf->blendVerts[b + 3] / 64;
				surf->blendVertsTable[a].b[3] = surf->blendVerts[b + 5] / 64;
				surf->blendVertsTable[a].b[4] = 0;
				surf->blendVertsTable[a].b[5] = 0;
				surf->blendVertsTable[a].b[6] = 0;
				surf->blendVertsTable[a].b[7] = 0;

				surf->blendVertsTable[a].b[8] = surf->blendVerts[b + 2];
				surf->blendVertsTable[a].b[9] = surf->blendVerts[b + 4];
				surf->blendVertsTable[a].b[10] = surf->blendVerts[b + 6];
				surf->blendVertsTable[a].b[11] = 0;
				surf->blendVertsTable[a].b[12] = 0;
				surf->blendVertsTable[a].b[13] = 0;
				surf->blendVertsTable[a].b[14] = 0;

				surf->blendVertsTable[a].blendVertCountIndex = index;

				a++;
				b += 7;
			}
			index++;
		}

		void GenerateH1XSurface(H1::XSurface* h1_asset, XSurface* asset, ZoneMemory* mem)
		{
			h1_asset->flags = 0;
			h1_asset->flags |= ((asset->flags & IW5::SURF_FLAG_VERTCOL_GREY) != 0) ? H1::SURF_FLAG_VERTCOL_GREY : 0;
			h1_asset->flags |= ((asset->flags & IW5::SURF_FLAG_VERTCOL_NONE) != 0) ? H1::SURF_FLAG_VERTCOL_NONE : 0;
			//h1_asset->flags |= ((asset->flags & IW5::SURF_FLAG_QUANTIZED) != 0) ? H1::SURF_FLAG_QUANTIZED : 0;
			h1_asset->flags |= ((asset->flags & IW5::SURF_FLAG_SKINNED) != 0) ? H1::SURF_FLAG_SKINNED : 0;

			h1_asset->vertCount = asset->vertCount;
			h1_asset->triCount = asset->triCount;
			h1_asset->rigidVertListCount = asset->vertListCount;

			// blend verts
			memcpy(&h1_asset->blendVertCounts, &asset->vertexInfo.vertCount, sizeof(asset->vertexInfo.vertCount));
			h1_asset->blendVerts = reinterpret_cast<unsigned short* __ptr64>(asset->vertexInfo.vertsBlend);

			h1_asset->blendVertsTable = mem->Alloc<H1::BlendVertsUnknown>(asset->vertCount);
			GenerateH1BlendVertsShit(h1_asset);

			// triIndices
			h1_asset->triIndices = reinterpret_cast<H1::Face * __ptr64>(asset->triIndices); // this is draw indices?
			h1_asset->triIndices2 = mem->Alloc<H1::Face>(asset->triCount); // this is collision indices?
			for (unsigned short i = 0; i < asset->triCount; i++)
			{
				memcpy(&h1_asset->triIndices2[i], &h1_asset->triIndices[i], sizeof(H1::Face));
			}

			// verts
			//h1_asset->verts0.packedVerts0 = reinterpret_cast<IW6::GfxPackedVertex* __ptr64>(asset->verticies);
			h1_asset->verts0.packedVerts0 = mem->Alloc<H1::GfxPackedVertex>(asset->vertCount);
			for (unsigned short i = 0; i < asset->vertCount; i++)
			{
				memcpy(&h1_asset->verts0.packedVerts0[i], &asset->verticies[i], sizeof(IW5::GfxPackedVertex));

				float texCoord_unpacked[2]{ 0.0f, 0.0f };
				PackedVec::Vec2UnpackTexCoords(asset->verticies[i].texCoord.packed, texCoord_unpacked);
				std::swap(texCoord_unpacked[0], texCoord_unpacked[1]); // these are inverted...
				h1_asset->verts0.packedVerts0[i].texCoord.packed = PackedVec::Vec2PackTexCoords(texCoord_unpacked);

				// re-calculate these...
				float normal_unpacked[3]{ 0.0f, 0.0f, 0.0f };
				PackedVec::Vec3UnpackUnitVec(asset->verticies[i].normal.array, normal_unpacked);

				float tangent_unpacked[3]{ 0.0f, 0.0f, 0.0f };
				PackedVec::Vec3UnpackUnitVec(asset->verticies[i].tangent.array, tangent_unpacked);

				float normal[3] = { normal_unpacked[0], normal_unpacked[1], normal_unpacked[2] };
				float tangent[3] = { tangent_unpacked[0], tangent_unpacked[1], tangent_unpacked[2] };

				float sign = 0.0f;
				if (asset->verticies[i].binormalSign == -1.0f)
				{
					sign = 1.0f;
				}

				h1_asset->verts0.packedVerts0[i].normal.packed = PackedVec::Vec3PackUnitVecWithAlpha(normal, 1.0f);
				h1_asset->verts0.packedVerts0[i].tangent.packed = PackedVec::Vec3PackUnitVecWithAlpha(tangent, sign);
				
				// correct color : bgra->rgba
				h1_asset->verts0.packedVerts0[i].color.array[0] = asset->verticies[i].color.array[2];
				h1_asset->verts0.packedVerts0[i].color.array[1] = asset->verticies[i].color.array[1];
				h1_asset->verts0.packedVerts0[i].color.array[2] = asset->verticies[i].color.array[0];
				h1_asset->verts0.packedVerts0[i].color.array[3] = asset->verticies[i].color.array[3];
			}

			// unknown
			h1_asset->unknown0 = mem->Alloc<H1::UnknownXSurface0>(asset->vertCount); // related to indices2?
			for (unsigned short i = 0; i < asset->vertCount; i++)
			{
				h1_asset->unknown0[i].xyz[0] = h1_asset->verts0.packedVerts0[i].xyz[0];
				h1_asset->unknown0[i].xyz[1] = h1_asset->verts0.packedVerts0[i].xyz[1];
				h1_asset->unknown0[i].xyz[2] = h1_asset->verts0.packedVerts0[i].xyz[2];
				h1_asset->unknown0[i].normal = h1_asset->verts0.packedVerts0[i].normal;
			}

			// rigidVertLists
			h1_asset->rigidVertLists = mem->Alloc<H1::XRigidVertList>(asset->vertListCount);
			for (int i = 0; i < asset->vertListCount; i++)
			{
				h1_asset->rigidVertLists[i].boneOffset = asset->rigidVertLists[i].boneOffset;
				h1_asset->rigidVertLists[i].vertCount = asset->rigidVertLists[i].vertCount;
				h1_asset->rigidVertLists[i].triOffset = asset->rigidVertLists[i].triOffset;
				h1_asset->rigidVertLists[i].triCount = asset->rigidVertLists[i].triCount;

				if (asset->rigidVertLists[i].collisionTree)
				{
					h1_asset->rigidVertLists[i].collisionTree = mem->Alloc<H1::XSurfaceCollisionTree>();
					memcpy(&h1_asset->rigidVertLists[i].collisionTree->trans, &asset->rigidVertLists[i].collisionTree->trans,
						sizeof(asset->rigidVertLists[i].collisionTree->trans));
					memcpy(&h1_asset->rigidVertLists[i].collisionTree->scale, &asset->rigidVertLists[i].collisionTree->scale,
						sizeof(asset->rigidVertLists[i].collisionTree->scale));

					h1_asset->rigidVertLists[i].collisionTree->nodeCount = asset->rigidVertLists[i].collisionTree->nodeCount;
					h1_asset->rigidVertLists[i].collisionTree->nodes = mem->Alloc<H1::XSurfaceCollisionNode>(
						asset->rigidVertLists[i].collisionTree->nodeCount);
					for (int j = 0; j < asset->rigidVertLists[i].collisionTree->nodeCount; j++)
					{
						memcpy(&h1_asset->rigidVertLists[i].collisionTree->nodes[j].aabb.mins,
							&asset->rigidVertLists[i].collisionTree->nodes[j].aabb.mins,
							sizeof(asset->rigidVertLists[i].collisionTree->nodes[j].aabb.mins));
						memcpy(&h1_asset->rigidVertLists[i].collisionTree->nodes[j].aabb.maxs,
							&asset->rigidVertLists[i].collisionTree->nodes[j].aabb.maxs,
							sizeof(asset->rigidVertLists[i].collisionTree->nodes[j].aabb.maxs));

						h1_asset->rigidVertLists[i].collisionTree->nodes[j].childBeginIndex =
							asset->rigidVertLists[i].collisionTree->nodes[j].childBeginIndex;
						h1_asset->rigidVertLists[i].collisionTree->nodes[j].childCount =
							asset->rigidVertLists[i].collisionTree->nodes[j].childCount;
					}

					h1_asset->rigidVertLists[i].collisionTree->leafCount = asset->rigidVertLists[i].collisionTree->leafCount;
					h1_asset->rigidVertLists[i].collisionTree->leafs = mem->Alloc<H1::XSurfaceCollisionLeaf>(
						asset->rigidVertLists[i].collisionTree->leafCount);
					for (int j = 0; j < asset->rigidVertLists[i].collisionTree->leafCount; j++)
					{
						h1_asset->rigidVertLists[i].collisionTree->leafs[j].triangleBeginIndex =
							asset->rigidVertLists[i].collisionTree->leafs[j].triangleBeginIndex;
					}
				}
			}

			// partBits
			memcpy(&h1_asset->partBits, &asset->partBits, sizeof(asset->partBits));
		}

		H1::XModelSurfs* GenerateH1XModelSurfs(ModelSurface* asset, ZoneMemory* mem)
		{
			// allocate H1 XModelSurfs structure
			const auto h1_asset = mem->Alloc<H1::XModelSurfs>();

			h1_asset->name = mem->StrDup(asset->name);
			h1_asset->numsurfs = asset->xSurficiesCount;
			memcpy(&h1_asset->partBits, &asset->partBits, sizeof(asset->partBits));

			h1_asset->surfs = mem->Alloc<H1::XSurface>(asset->xSurficiesCount);
			for (int i = 0; i < asset->xSurficiesCount; i++)
			{
				GenerateH1XSurface(&h1_asset->surfs[i], &asset->xSurficies[i], mem);
			}

			return h1_asset;
		}

		void IXSurface::dump(ModelSurface* asset, ZoneMemory* mem)
		{
			// generate h1 surfaces
			auto h1_asset = GenerateH1XModelSurfs(asset, mem);

			// dump h1 surfaces
			H1::IXSurface::dump(h1_asset);
		}
	}
}