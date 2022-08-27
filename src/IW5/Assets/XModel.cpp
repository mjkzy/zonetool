#include "stdafx.hpp"
#include "H1/Assets/XModel.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		H1::XModel* GenerateH1Model(XModel* asset, ZoneMemory* mem)
		{
			// allocate H1 XModel structure
			auto* h1_asset = mem->Alloc<H1::XModel>();

			h1_asset->name = asset->name;
			h1_asset->numBones = asset->numBones;
			h1_asset->numRootBones = asset->numRootBones;
			h1_asset->numsurfs = asset->numSurfaces;
			h1_asset->numReactiveMotionParts = 0;
			h1_asset->lodRampType = asset->lodRampType;
			h1_asset->scale = 1.0f;
			memcpy(&h1_asset->noScalePartBits, &asset->noScalePartBits, sizeof(asset->noScalePartBits));

			h1_asset->boneNames = mem->Alloc<H1::scr_string_t>(asset->numBones);
			for (auto i = 0; i < asset->numBones; i++)
			{
				h1_asset->boneNames[i] = static_cast<H1::scr_string_t>(asset->boneNames[i]);
			}

			REINTERPRET_CAST_SAFE(h1_asset->parentList, asset->parentList);
			REINTERPRET_CAST_SAFE(h1_asset->tagAngles, asset->tagAngles);
			REINTERPRET_CAST_SAFE(h1_asset->tagPositions, asset->tagPositions);
			REINTERPRET_CAST_SAFE(h1_asset->partClassification, asset->partClassification);
			REINTERPRET_CAST_SAFE(h1_asset->baseMat, asset->animMatrix);
			h1_asset->reactiveMotionParts = nullptr;
			h1_asset->reactiveMotionTweaks = nullptr;

			h1_asset->materialHandles = mem->Alloc<H1::Material* __ptr64>(asset->numSurfaces);
			for (auto i = 0; i < asset->numSurfaces; i++)
			{
				if (asset->materials[i])
				{
					h1_asset->materialHandles[i] = mem->Alloc<H1::Material>();
					h1_asset->materialHandles[i]->name = mem->StrDup(asset->materials[i]->name);
				}
			}

			// level of detail data
			for (int i = 0; i < asset->numLods; i++)
			{
				h1_asset->lodInfo[i].dist = asset->lods[i].dist;
				h1_asset->lodInfo[i].numsurfs = asset->lods[i].numSurfacesInLod;
				h1_asset->lodInfo[i].surfIndex = asset->lods[i].surfIndex;
				h1_asset->lodInfo[i].modelSurfs = mem->Alloc<H1::XModelSurfs>(asset->lods[i].numSurfacesInLod);
				for (int j = 0; j < asset->lods[i].numSurfacesInLod; j++)
				{
					h1_asset->lodInfo[i].modelSurfs[j].name = mem->StrDup(asset->lods[i].surfaces[j].name);
				}
				memcpy(&h1_asset->lodInfo[i].partBits, &asset->lods[i].partBits, sizeof(asset->lods[i].partBits));
			}

			//h1_asset->maxLoadedLod = asset->maxLoadedLod;
			h1_asset->numLods = asset->numLods;
			h1_asset->collLod = asset->collLod;
			h1_asset->numCompositeModels = 0;
			h1_asset->flags = asset->flags;

			h1_asset->numCollSurfs = asset->numColSurfs;
			h1_asset->collSurfs = mem->Alloc<H1::XModelCollSurf_s>(asset->numColSurfs);
			for (auto i = 0; i < asset->numColSurfs; i++)
			{
				memcpy(&h1_asset->collSurfs[i].bounds, &asset->colSurf[i].mins, sizeof(float[2][3]));

				h1_asset->collSurfs[i].boneIdx = asset->colSurf[i].boneIdx;
				h1_asset->collSurfs[i].contents = asset->colSurf[i].contents;
				h1_asset->collSurfs[i].surfFlags = asset->colSurf[i].surfFlags;
			}

			h1_asset->contents = asset->contents;

			REINTERPRET_CAST_SAFE(h1_asset->boneInfo, asset->boneInfo);

			h1_asset->radius = asset->radius;
			memcpy(&h1_asset->bounds, &asset->bounds, sizeof(asset->bounds));
			h1_asset->memUsage = asset->memUsage;

			if (asset->physPreset)
			{
				h1_asset->physPreset = mem->Alloc<H1::PhysPreset>();
				h1_asset->physPreset->name = mem->StrDup(asset->physPreset->name);
			}

			if (asset->physCollmap)
			{
				h1_asset->physCollmap = mem->Alloc<H1::PhysCollmap>();
				h1_asset->physCollmap->name = mem->StrDup(asset->physCollmap->name);
			}

			// idk
			h1_asset->invHighMipRadius = mem->Alloc<unsigned short>(asset->numSurfaces);
			memset(h1_asset->invHighMipRadius, 0, sizeof(unsigned short) * asset->numSurfaces);

			h1_asset->quantization = 0.0f; //1.0f;

			return h1_asset;
		}

		void IXModel::dump(XModel* asset, ZoneMemory* mem)
		{
			// generate h1 model
			auto h1_asset = GenerateH1Model(asset, mem);

			// dump h1 model
			H1::IXModel::dump(h1_asset, SL_ConvertToString);
		}
	}
}