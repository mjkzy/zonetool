#include "stdafx.hpp"
#include "../Include.hpp"

#include "XModel.hpp"
#include "Assets/Material.hpp"

namespace ZoneTool::IW5
{
	namespace IW7Converter
	{
		namespace
		{
			/*IW7::CSurfaceFlags surf_flags_conversion_table[31]
			{
				IW7::SURF_FLAG_DEFAULT,
				IW7::SURF_FLAG_BARK,
				IW7::SURF_FLAG_BRICK,
				IW7::SURF_FLAG_CARPET,
				IW7::SURF_FLAG_CLOTH,
				IW7::SURF_FLAG_CONCRETE,
				IW7::SURF_FLAG_DIRT,
				IW7::SURF_FLAG_FLESH,
				IW7::SURF_FLAG_FOLIAGE,
				IW7::SURF_FLAG_GLASS,
				IW7::SURF_FLAG_GRASS,
				IW7::SURF_FLAG_GRAVEL,
				IW7::SURF_FLAG_ICE,
				IW7::SURF_FLAG_METAL,
				IW7::SURF_FLAG_MUD,
				IW7::SURF_FLAG_PAPER,
				IW7::SURF_FLAG_PLASTER,
				IW7::SURF_FLAG_ROCK,
				IW7::SURF_FLAG_SAND,
				IW7::SURF_FLAG_SNOW,
				IW7::SURF_FLAG_WATER,
				IW7::SURF_FLAG_WOOD,
				IW7::SURF_FLAG_ASPHALT,
				IW7::SURF_FLAG_CERAMIC,
				IW7::SURF_FLAG_PLASTIC,
				IW7::SURF_FLAG_RUBBER,
				IW7::SURF_FLAG_CUSHION,
				IW7::SURF_FLAG_FRUIT,
				IW7::SURF_FLAG_PAINTEDMETAL,
				IW7::SURF_FLAG_RIOTSHIELD,
				IW7::SURF_FLAG_SLUSH,
			}; IW5::CSurfaceFlags;*/

			/*int convert_surf_flags(int flags)
			{
				int IW7_flags = surf_flags_conversion_table[flags >> 20];
				auto convert = [&](IW5::CSurfaceFlags a, IW7::CSurfaceFlags b)
				{
					IW7_flags |= ((flags & a) == a) ? b : 0;
				};
				convert(IW5::CSurfaceFlags::SURF_FLAG_OPAQUEGLASS, IW7::CSurfaceFlags::SURF_FLAG_DEFAULT);
				convert(IW5::CSurfaceFlags::SURF_FLAG_CLIPMISSILE, IW7::CSurfaceFlags::SURF_FLAG_CLIPMISSILE);
				convert(IW5::CSurfaceFlags::SURF_FLAG_AI_NOSIGHT, IW7::CSurfaceFlags::SURF_FLAG_AI_NOSIGHT);
				convert(IW5::CSurfaceFlags::SURF_FLAG_CLIPSHOT, IW7::CSurfaceFlags::SURF_FLAG_CLIPSHOT);
				convert(IW5::CSurfaceFlags::SURF_FLAG_PLAYERCLIP, IW7::CSurfaceFlags::SURF_FLAG_PLAYERCLIP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_MONSTERCLIP, IW7::CSurfaceFlags::SURF_FLAG_MONSTERCLIP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_AICLIPALLOWDEATH, IW7::CSurfaceFlags::SURF_FLAG_AICLIPALLOWDEATH);
				convert(IW5::CSurfaceFlags::SURF_FLAG_VEHICLECLIP, IW7::CSurfaceFlags::SURF_FLAG_VEHICLECLIP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_ITEMCLIP, IW7::CSurfaceFlags::SURF_FLAG_ITEMCLIP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NODROP, IW7::CSurfaceFlags::SURF_FLAG_NODROP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NONSOLID, IW7::CSurfaceFlags::SURF_FLAG_NONSOLID);
				convert(IW5::CSurfaceFlags::SURF_FLAG_DETAIL, IW7::CSurfaceFlags::SURF_FLAG_DETAIL);
				convert(IW5::CSurfaceFlags::SURF_FLAG_STRUCTURAL, IW7::CSurfaceFlags::SURF_FLAG_STRUCTURAL);
				convert(IW5::CSurfaceFlags::SURF_FLAG_PORTAL, IW7::CSurfaceFlags::SURF_FLAG_PORTAL);
				convert(IW5::CSurfaceFlags::SURF_FLAG_CANSHOOTCLIP, IW7::CSurfaceFlags::SURF_FLAG_CANSHOOTCLIP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_ORIGIN, IW7::CSurfaceFlags::SURF_FLAG_ORIGIN);
				convert(IW5::CSurfaceFlags::SURF_FLAG_SKY, IW7::CSurfaceFlags::SURF_FLAG_SKY);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOCASTSHADOW, IW7::CSurfaceFlags::SURF_FLAG_NOCASTSHADOW);
				convert(IW5::CSurfaceFlags::SURF_FLAG_PHYSICSGEOM, IW7::CSurfaceFlags::SURF_FLAG_PHYSICSGEOM);
				convert(IW5::CSurfaceFlags::SURF_FLAG_LIGHTPORTAL, IW7::CSurfaceFlags::SURF_FLAG_LIGHTPORTAL);
				convert(IW5::CSurfaceFlags::SURF_FLAG_OUTDOORBOUNDS, IW7::CSurfaceFlags::SURF_FLAG_OUTDOORBOUNDS);
				convert(IW5::CSurfaceFlags::SURF_FLAG_SLICK, IW7::CSurfaceFlags::SURF_FLAG_SLICK);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOIMPACT, IW7::CSurfaceFlags::SURF_FLAG_NOIMPACT);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOMARKS, IW7::CSurfaceFlags::SURF_FLAG_NOMARKS);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOPENETRATE, IW7::CSurfaceFlags::SURF_FLAG_NOPENETRATE);
				convert(IW5::CSurfaceFlags::SURF_FLAG_LADDER, IW7::CSurfaceFlags::SURF_FLAG_LADDER);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NODAMAGE, IW7::CSurfaceFlags::SURF_FLAG_NODAMAGE);
				convert(IW5::CSurfaceFlags::SURF_FLAG_MANTLEON, IW7::CSurfaceFlags::SURF_FLAG_MANTLEON);
				convert(IW5::CSurfaceFlags::SURF_FLAG_MANTLEOVER, IW7::CSurfaceFlags::SURF_FLAG_MANTLEOVER);
				convert(IW5::CSurfaceFlags::SURF_FLAG_STAIRS, IW7::CSurfaceFlags::SURF_FLAG_STAIRS);
				convert(IW5::CSurfaceFlags::SURF_FLAG_SOFT, IW7::CSurfaceFlags::SURF_FLAG_SOFT);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOSTEPS, IW7::CSurfaceFlags::SURF_FLAG_NOSTEPS);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NODRAW, IW7::CSurfaceFlags::SURF_FLAG_NODRAW);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NOLIGHTMAP, IW7::CSurfaceFlags::SURF_FLAG_NOLIGHTMAP);
				convert(IW5::CSurfaceFlags::SURF_FLAG_NODLIGHT, IW7::CSurfaceFlags::SURF_FLAG_NODLIGHT);
				return IW7_flags;
			}*/
		}

		IW7::XModel* GenerateIW7Model(XModel* asset, allocator& mem)
		{
			// allocate IW7 XModel structure
			auto* IW7_asset = mem.allocate<IW7::XModel>();

			IW7_asset->name = asset->name;
			IW7_asset->numBones = asset->numBones;
			IW7_asset->numRootBones = asset->numRootBones;
			IW7_asset->numsurfs = asset->numsurfs;
			IW7_asset->numReactiveMotionParts = 0;
			IW7_asset->scale = asset->scale;
			memcpy(&IW7_asset->noScalePartBits, &asset->noScalePartBits, sizeof(asset->noScalePartBits));

			IW7_asset->boneNames = mem.allocate<IW7::scr_string_t>(asset->numBones);
			for (auto i = 0; i < asset->numBones; i++)
			{
				IW7_asset->boneNames[i] = static_cast<IW7::scr_string_t>(asset->boneNames[i]);
			}

			REINTERPRET_CAST_SAFE(IW7_asset->parentList, asset->parentList);
			REINTERPRET_CAST_SAFE(IW7_asset->tagAngles, asset->quats);
			REINTERPRET_CAST_SAFE(IW7_asset->tagPositions, asset->trans);
			REINTERPRET_CAST_SAFE(IW7_asset->partClassification, asset->partClassification);
			REINTERPRET_CAST_SAFE(IW7_asset->baseMat, asset->baseMat);
			IW7_asset->reactiveMotionParts = nullptr;

			IW7_asset->materialHandles = mem.allocate<IW7::Material* __ptr64>(asset->numsurfs);
			for (auto i = 0; i < asset->numsurfs; i++)
			{
				if (asset->materialHandles[i])
				{
					IW7_asset->materialHandles[i] = mem.allocate<IW7::Material>();
					IW7_asset->materialHandles[i]->name = mem.duplicate_string(IW7::replace_material_prefix(asset->materialHandles[i]->info.name));
				}
			}

			for (auto i = 0; i < 6; i++)
			{
				IW7_asset->lodInfo[i].dist = 1000000.0f;
			}

			// level of detail data
			for (auto i = 0; i < asset->numLods; i++)
			{
				IW7_asset->lodInfo[i].dist = asset->lodInfo[i].dist;
				IW7_asset->lodInfo[i].numsurfs = asset->lodInfo[i].numsurfs;
				IW7_asset->lodInfo[i].surfIndex = asset->lodInfo[i].surfIndex;
				IW7_asset->lodInfo[i].modelSurfs = mem.allocate<IW7::XModelSurfs>(asset->lodInfo[i].numsurfs);
				IW7_asset->lodInfo[i].modelSurfs->name = mem.duplicate_string(asset->lodInfo[i].modelSurfs->name);
				memcpy(&IW7_asset->lodInfo[i].partBits, &asset->lodInfo[i].partBits, sizeof(asset->lodInfo[i].partBits));
			}

			IW7_asset->unknown01 = asset->maxLoadedLod; //IW7_asset->maxLoadedLod = asset->maxLoadedLod;
			IW7_asset->numLods = asset->numLods;
			IW7_asset->collLod = asset->collLod;
			IW7_asset->flags = asset->flags;

			IW7_asset->numCollSurfs = asset->numCollSurfs;
			IW7_asset->collSurfs = mem.allocate<IW7::XModelCollSurf_s>(asset->numCollSurfs);
			for (auto i = 0; i < asset->numCollSurfs; i++)
			{
				memcpy(&IW7_asset->collSurfs[i].bounds, &asset->collSurfs[i].bounds, sizeof(float[2][3]));

				IW7_asset->collSurfs[i].boneIdx = asset->collSurfs[i].boneIdx;
				IW7_asset->collSurfs[i].contents = asset->collSurfs[i].contents;
				IW7_asset->collSurfs[i].surfFlags = 0;//convert_surf_flags(asset->collSurfs[i].surfFlags);
			}

			IW7_asset->contents = asset->contents;

			REINTERPRET_CAST_SAFE(IW7_asset->boneInfo, asset->boneInfo);

			IW7_asset->radius = asset->radius;
			memcpy(&IW7_asset->bounds, &asset->bounds, sizeof(asset->bounds));
			IW7_asset->memUsage = asset->memUsage;

			if (asset->physPreset)
			{
				//IW7_asset->physAsset = mem.allocate<IW7::PhysicsAsset>();
				//IW7_asset->physAsset->name = mem.duplicate_string(asset->physPreset->name);
			}

			if (asset->physCollmap)
			{
				// ?
				//IW7_asset->physFxShape = mem.allocate<IW7::PhysicsFXShape>();
				//IW7_asset->physFxShape->name = mem.duplicate_string(asset->physCollmap->name);
			}

			// idk
			IW7_asset->invHighMipRadius = mem.allocate<unsigned short>(asset->numsurfs);
			for (unsigned char i = 0; i < asset->numsurfs; i++)
			{
				IW7_asset->invHighMipRadius[i] = 0xFFFF;
			}

			//IW7_asset->quantization = 0.0f;

			return IW7_asset;
		}

		IW7::XModel* convert(XModel* asset, allocator& allocator)
		{
			// generate IW7 model
			return GenerateIW7Model(asset, allocator);
		}
	}
}