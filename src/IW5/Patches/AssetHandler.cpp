// ======================= ZoneTool =======================
// zonetool, a fastfile linker for various
// Call of Duty titles. 
//
// Project: https://github.com/ZoneTool/zonetool
// Author: RektInator (https://github.com/RektInator)
// License: GNU GPL v3.0
// ========================================================
#include "stdafx.hpp"

namespace ZoneTool
{
	namespace IW5
	{
#ifdef DEBUG
		constexpr auto IS_DEBUG = true;
#else
		constexpr auto IS_DEBUG = false;
#endif

		extern bool is_dumping_complete;
		extern bool is_dumping;

		XAssetEntry AssetHandler::XAssetEntries[AssetEntries];
		std::uint32_t AssetHandler::db_hashmap[AssetEntries];

		bool AssetHandler::verify = false;
		bool AssetHandler::dump = false;

		std::string AssetHandler::fastfile;
		FILE* AssetHandler::csvFile;

		void** AssetHandler::DB_XAssetPool = (void**)0x7C5E38;
		unsigned int* AssetHandler::g_poolSize = (unsigned int*)0x7C5B58;
		AssetHandler::DB_GetXAssetSizeHandler_t* AssetHandler::DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)
			0x7C6430;

		std::unordered_map<std::string, XAsset> AssetHandler::StoredAssets;
		std::vector<std::pair<XAssetType, std::string>> AssetHandler::referencedAssets;

		XAssetHeader AssetHandler::FindXAsset(std::int32_t type, const char* name, std::uint32_t unk)
		{
			if (StoredAssets.find(name) != StoredAssets.end())
			{
				if (StoredAssets[name].type == type)
				{
					return StoredAssets[name].header;
				}
			}

			return XAssetHeader(); // DB_FindXAssetHeader(type, name, flags);
		}

		void AssetHandler::AddXAsset(std::int32_t type, std::string name, void* pointer)
		{
			if (StoredAssets.find(name) == StoredAssets.end())
			{
				StoredAssets[name] = {static_cast<XAssetType>(type), reinterpret_cast<RawFile*>(pointer)};
			}
		}

		void AssetHandler::SetVerify(bool state)
		{
			verify = state;
		}

		void AssetHandler::SetDump(bool state)
		{
			dump = state;
		}

		const char* GetAssetName(std::int32_t type, void* ptr)
		{
			if (type == ASSET_TYPE_IMAGE)
			{
				return reinterpret_cast<GfxImage*>(ptr)->name;
			}

			if (type == ASSET_TYPE_LOCALIZE_ENTRY)
			{
				return reinterpret_cast<LocalizeEntry*>(ptr)->name;
			}

			if (type == ASSET_TYPE_MENU)
			{
				return reinterpret_cast<menuDef_t*>(ptr)->window.name;
			}

			return reinterpret_cast<RawFile*>(ptr)->name;
		}

		static std::vector<std::pair<std::int32_t, std::string>> CommonAssets;

		void* DB_FindXAssetHeader_Unsafe(const XAssetType type, const std::string& name)
		{
			const static auto DB_FindXAssetHeader_Internal = 0x44E540;
			const auto name_ptr = name.data();
			const auto type_int = static_cast<std::int32_t>(type);

			const XAsset* asset_header = nullptr;

			__asm
			{
				mov edi, name_ptr;
				push type_int;
				call DB_FindXAssetHeader_Internal;
				add esp, 4;
				mov asset_header, eax;
			}

			return (asset_header) ? asset_header->header.data : nullptr;
		}

		XAssetHeader DB_FindXAssetHeader_(std::int32_t type, const char* name, bool createDefault)
		{
			auto retval = DB_FindXAssetHeader(type, name, createDefault);
			if (createDefault || retval.data == nullptr)
			{
				ZONETOOL_INFO("MISSING ASSET '%s' of type '%s'", name, reinterpret_cast<char**>(
					0x7C6208)[type]);
			}

			return retval;
		}

		void AssetHandler::StopDump()
		{
			ZONETOOL_INFO("Zone \"%s\" dumped.", &fastfile[0]);

			referencedAssets.clear();
			FileSystem::FileClose(csvFile);
			csvFile = nullptr;

			is_dumping_complete = true;
		}
		
		void AssetHandler::DB_LogLoadedAsset(void* ptr, std::int32_t type)
		{
#ifdef USE_VMPROTECT
			VMProtectBeginUltra("IW5::DB_LogLoadedAsset");
#endif
			static std::vector<std::pair<XAssetType, std::string>> referencedAssets;

			fastfile = static_cast<std::string>(reinterpret_cast<const char*>(*reinterpret_cast<DWORD*>(0x1294A00)
				+ 4));

			// store all common assets
			if (fastfile == "common_mp")
			{
				CommonAssets.push_back({type, GetAssetName(type, ptr)});
			}

			if (dump)
			{
				FileSystem::SetFastFile(fastfile);
				zonetool::filesystem::set_fastfile(fastfile);

				// open csv file for dumping 
				if (!csvFile)
				{
					csvFile = FileSystem::FileOpen(fastfile + ".csv", "wb");
				}

				// dump assets to disk
				if (csvFile)
				{
					auto xassettypes = reinterpret_cast<char**>(0x7C6208);
					fprintf(csvFile, "%s,%s\n", xassettypes[type], GetAssetName(type, ptr));
				}

				// check if we're done loading the fastfile
				if (type == rawfile && GetAssetName(type, ptr) == fastfile)
				{
					for (auto& asset : referencedAssets)
					{
						if (asset.second.length() <= 1 || asset.first == XAssetType_old::loaded_sound)
						{
							continue;
						}

						const auto asset_name = &asset.second[1];
						const auto ref_asset = DB_FindXAssetHeader_Unsafe(asset.first, asset_name);

						if (ref_asset == nullptr)
						{
							ZONETOOL_ERROR("Could not find referenced asset \"%s\"!", asset_name);
							continue;
						}

						ZONETOOL_INFO("Dumping additional asset \"%s\" because it is referenced by %s.", asset_name, fastfile.data());

						if (type == XAssetType_old::xmodel)
						{
							auto* xmodel = reinterpret_cast<XModel*>(ref_asset);
							for (auto i = 0; i < xmodel->numsurfs; i++)
							{
								XAsset material_asset;
								material_asset.type = ASSET_TYPE_MATERIAL;
								material_asset.header.material = xmodel->materialHandles[i];
								DB_LogLoadedAsset(material_asset.header.data, material_asset.type);
							}
							for (auto i = 0; i < xmodel->numLods; i++)
							{
								XAsset surface_asset;
								surface_asset.type = ASSET_TYPE_XMODEL_SURFS;
								surface_asset.header.modelSurfs = xmodel->lodInfo[i].modelSurfs;
								DB_LogLoadedAsset(surface_asset.header.data, surface_asset.type);
							}
						}

						DB_LogLoadedAsset(ref_asset, asset.first);
					}

					StopDump();
				}

				if (GetAssetName(type, ptr)[0] == ',')
				{
					referencedAssets.push_back({ XAssetType(type), GetAssetName(type, ptr) });
				}
				else
				{
#define DUMPCASE(__type__,__interface__,__struct__) \
				if (type == __type__) \
				{ \
					auto asset = reinterpret_cast<__struct__*>(ptr); \
					__interface__::dump(asset); \
				}
					try
					{
						DUMPCASE(col_map_mp, IClipMap, clipMap_t);
						DUMPCASE(com_map, IComWorld, ComWorld);
						DUMPCASE(fx, IFxEffectDef, FxEffectDef);
						DUMPCASE(fx_map, IFxWorld, FxWorld);
						DUMPCASE(image, IGfxImage, GfxImage);
						DUMPCASE(lightdef, IGfxLightDef, GfxLightDef);
						DUMPCASE(gfx_map, IGfxWorld, GfxWorld);
						DUMPCASE(glass_map, IGlassWorld, GlassWorld);
						DUMPCASE(loaded_sound, ILoadedSound, LoadedSound);
						DUMPCASE(map_ents, IMapEnts, MapEnts);
						DUMPCASE(material, IMaterial, Material);
						DUMPCASE(phys_collmap, IPhysCollmap, PhysCollmap);
						DUMPCASE(physpreset, IPhysPreset, PhysPreset);
						DUMPCASE(rawfile, IRawFile, RawFile);
						DUMPCASE(scriptfile, IScriptFile, ScriptFile);
						DUMPCASE(sound, ISound, snd_alias_list_t);
						DUMPCASE(sndcurve, ISoundCurve, SndCurve);
						DUMPCASE(xmodel, IXModel, XModel);
						DUMPCASE(xmodelsurfs, IXSurface, XModelSurfs);
						DUMPCASE(xanim, IXAnimParts, XAnimParts);
					}
					catch (std::exception& ex)
					{
						ZONETOOL_FATAL("A fatal exception occured while dumping asset \"%s\", exception was: %s\n", GetAssetName(type, ptr), ex.what());
					}
				}
			}
			else
			{
				/*std::string fastfile = reinterpret_cast<const char*>(*reinterpret_cast<DWORD*>(0x1294A00) + 4);
				if (fastfile.find("mp_") != std::string::npos || fastfile == "common_mp"s)
				{
					FileSystem::SetFastFile("");

					// dump everything techset related!
					DUMPCASE(material, IMaterial, Material);
					DUMPCASE(techset, ITechset, MaterialTechniqueSet);
					DUMPCASE(pixelshader, IPixelShader, PixelShader);
					DUMPCASE(vertexshader, IVertexShader, VertexShader);
					DUMPCASE(vertexdecl, IVertexDecl, VertexDecl);
				}*/
			}

			if (verify/* || FFCompression::ff_version > 1*/)
			{
				ZONETOOL_INFO("Loading asset \"%s\" of type %s.", GetAssetName(type, ptr), reinterpret_cast<char**>(
0x7C6208)[type]);
			}

#ifdef USE_VMPROTECT
			VMProtectEnd();
#endif
		}

		bool AssetHandler::IsCommonAsset(std::int32_t type, const std::string& name)
		{
			for (auto& commonAsset : CommonAssets)
			{
				if (commonAsset.first == type && commonAsset.second == name)
				{
					return true;
				}
			}

			return false;
		}

		__declspec(naked) void AssetHandler::DB_AddXAsset()
		{
			__asm
			{
				pushad;

				push ebp;
				push ecx;
				call AssetHandler::DB_LogLoadedAsset;
				add esp, 8;

				popad;

				jmp_back:
				mov eax, DWORD PTR[ebp * 4 + 0x7c62c0];

				push 0x44EFF6;
				retn;
			}
		}

		void* AssetHandler::ReallocateAssetPool(uint32_t type, unsigned int newSize)
		{
			int elSize = DB_GetXAssetSizeHandlers[type]();

			void* poolEntry = malloc(newSize * elSize);
			DB_XAssetPool[type] = poolEntry;
			g_poolSize[type] = newSize;

			return poolEntry;
		}

		void* AssetHandler::ReallocateAssetPoolM(uint32_t type, int multiplier)
		{
			int elSize = DB_GetXAssetSizeHandlers[type]();
			int newSize = multiplier * g_poolSize[type];

			void* poolEntry = malloc(newSize * elSize);
			DB_XAssetPool[type] = poolEntry;
			g_poolSize[type] = newSize;

			return poolEntry;
		}

		__declspec(naked) void AssetHandler::FixupAssetLoading()
		{
			__asm
			{
				cmp edi, 0;
				je asset_null;

				cmp byte ptr[edi + 8], 0;
				je asset_continue;

				push 0x44EE2E;
				retn;

				asset_continue:
				push 0x44EF00;
				retn;

				asset_null:
				pop edi;
				pop esi;
				pop ebp;
				pop ebx;
				pop ecx;
				retn;
			}
		}

		void logEntry(std::uint32_t entry)
		{
			ZONETOOL_INFO("Hashmap entry is %u.", entry);
		}

		__declspec(naked) void TestHashMap()
		{
			static std::uintptr_t calladdr = 0x44E360;

			__asm
			{
				call calladdr;

				pushad;

				push eax;
				call logEntry;
				add esp, 4;

				popad;

				push 0x44F026;
				retn;
			}
		}

		std::uintptr_t initialZoneStreams[9];

		void InitLogStreamPos()
		{
			auto g_streamPos = reinterpret_cast<std::uintptr_t*>(0x016634FC);

			// ZONETOOL_INFO("Setting initial zone stream positions...");

			for (int i = 0; i < 9; i++)
			{
				initialZoneStreams[i] = g_streamPos[i];
			}
		}

		void LogStreamPos()
		{
			auto fastfile = static_cast<std::string>(reinterpret_cast<const char*>(*reinterpret_cast<DWORD*>(0x1294A00)
				+ 4));

			auto g_streamPos = reinterpret_cast<std::uintptr_t*>(0x016634FC);
			auto g_streamPosArr = reinterpret_cast<std::uintptr_t*>(0x01683304);

			if (fastfile.find("mp_") != std::string::npos)
			{
				for (auto i = 0u; i < 9; i++)
				{
					// ZONETOOL_INFO("Stream %i: 0x%08X 0x%08X", i, g_streamPos[i], g_streamPosArr[i]);
				}
			}
		}

		__declspec(naked) void ShitOutTheCurrentStreamPositionsStub()
		{
			static std::uintptr_t DB_AuthLoad_InflateEnd = 0x004368E0;

			__asm
			{
				// log the fuck out of this shit
				pushad;
				call LogStreamPos;
				popad;

				// call original function
				call DB_AuthLoad_InflateEnd;

				// jump back
				push 0x00436CAD;
				retn;
			}
		}

		__declspec(naked) void InitStreamsStub()
		{
			static std::uintptr_t original = 0x00436970;

			__asm
			{
				pushad;
				call InitLogStreamPos;
				popad;

				call original;

				push 0x00436C0C;
				retn;
			}
		}

		const unsigned int textureBufferSize = 1024 * 1024 * 64 * 4; // 64mb
		unsigned int textureBufferIndex = 0;
		std::unordered_map<std::string, unsigned int> textureMap;
		std::string texturesFastfiles;

		char* GetTextureBuffer()
		{
			static char textureBuffer[textureBufferSize];
			return textureBuffer;
		}

		void ClearTextures()
		{
			// fastfile name
			auto fastfile = static_cast<std::string>(reinterpret_cast<const char*>(*reinterpret_cast<DWORD*>(0x1294A00)
				+ 4));
			if (fastfile != texturesFastfiles)
			{
				auto* buffer = GetTextureBuffer();

				std::memset(buffer, 0, textureBufferSize);
				textureBufferIndex = 0;
				textureMap.clear();

				texturesFastfiles = fastfile;
			}
		}

		void StoreTexture()
		{
			reinterpret_cast<void(*)()>(0x439970)();

			GfxImageLoadDef* loadDef = *reinterpret_cast<GfxImageLoadDef**>(0x1294D6C);
			GfxImage* image = *reinterpret_cast<GfxImage**>(0x1294DC0);

			ClearTextures();

			if (!loadDef->resourceSize)
				return;

			auto* buffer = GetTextureBuffer();

			if (textureMap.find(image->name) != textureMap.end())
			{
				void* data = &buffer[textureMap.at(image->name)];
				image->texture.loadDef = reinterpret_cast<GfxImageLoadDef*>(data);
				return;
			}
			textureMap[image->name] = textureBufferIndex;

			size_t size = 16 + loadDef->resourceSize;
			void* data = &buffer[textureBufferIndex];
			textureBufferIndex += size;

			if (textureBufferIndex >= textureBufferSize)
			{
				ZONETOOL_FATAL("IW5 Texture Buffer exceeded %imb/%imb",
					(textureBufferIndex / 1024 / 1024), (textureBufferSize / 1024 / 1024));
			}

			std::memcpy(data, loadDef, size);

			image->texture.loadDef = reinterpret_cast<GfxImageLoadDef*>(data);
		}

		void Alloc_MssSound()
		{
			MssSound* sound = *reinterpret_cast<MssSound**>(0x01294E30);

			// Z_MallocInternal
			char* data_ptr = reinterpret_cast<char* (*)(int)>(0x0052B460)(
				sound->info.data_len);

			char* temp_ptr = sound->data;

			sound->data = data_ptr;
			memcpy(sound->data, temp_ptr, sound->info.data_len);
		}

		__declspec(naked) void MssSound_ReadXFile_stub()
		{
			static std::uintptr_t origFunc = 0x00436F20;
			__asm
			{
				// call original function first
				call origFunc;

				pushad;
				call Alloc_MssSound;
				popad;

				push 0x00438556;
				retn;
			}
		}

		AssetHandler::AssetHandler()
		{
#ifdef USE_VMPROTECT
			VMProtectBeginUltra("IW5::AssetHandler");
#endif

			// Alloc zonememory
			// mem = std::make_shared < ZoneMemory >();

			// Stream position logging
			Memory(0x00436CA8).jump(ShitOutTheCurrentStreamPositionsStub);
			Memory(0x00436C07).jump(InitStreamsStub);

#ifdef REALLOC_XASSETS
			// Empty the arrays
			memset(AssetHandler::XAssetEntries, 0,	sizeof(AssetHandler::XAssetEntries));
			memset(AssetHandler::db_hashmap, 0,		sizeof(AssetHandler::db_hashmap));

			// Hashmap entry checking
			Memory(0x44F021).Jump(TestHashMap);

			// Reallocate XAssetEntries
			[]()
			{
				for (std::uint32_t addr = 0x401000; addr < 0x700000; addr++)
				{
					// realloc xassetentries
					if (*reinterpret_cast<std::uint32_t*>(addr) == 0x14C91D0)
					{
						// ZONETOOL_INFO("Address 0x%08X patched.", addr);
						Memory(addr).Set(XAssetEntries);
					}

					// realloc hashmap
					if (*reinterpret_cast<std::uint32_t*>(addr) == 0x1381FB8)
					{
						// Memory(addr).Set(db_hashmap);
					}
				}

				Memory(0x133DA9C).Set(AssetEntries);
			}();

			// Reallocate XAssetEntry poolsize
			Memory(0x44E440).Set(AssetEntries);
			Memory(0x44EBF4).Set(AssetEntries);
			Memory(0x4507F5).Set(AssetEntries);
			Memory(0x4508A8).Set(AssetEntries);
			Memory(0x450908).Set(AssetEntries);
			Memory(0x450D42).Set(AssetEntries);
			Memory(0x44EBF2).Set(AssetEntries);
			Memory(0x4508A8).Set(AssetEntries);
			Memory(0x450908).Set(AssetEntries);
			Memory(0x450D50).Set(AssetEntries);
			Memory(0x44E3A8).Set(AssetEntries);
#endif

			// Always allocate a default asset, even for map assets.
			Memory(0x4505EE).nop(6);

			// Reallocate AssetPools
			ReallocateAssetPoolM(techset, 2);
			ReallocateAssetPoolM(map_ents, 5);
			ReallocateAssetPoolM(com_map, 5);
			ReallocateAssetPoolM(col_map_mp, 5);
			ReallocateAssetPoolM(gfx_map, 5);
			ReallocateAssetPoolM(fx_map, 5);
			ReallocateAssetPoolM(glass_map, 5);
			ReallocateAssetPoolM(localize, 2);
			ReallocateAssetPoolM(material, 2);
			ReallocateAssetPoolM(font, 2);
			ReallocateAssetPoolM(image, 2);
			ReallocateAssetPoolM(fx, 2);
			ReallocateAssetPoolM(xanim, 2);
			ReallocateAssetPoolM(xmodel, 2);
			ReallocateAssetPoolM(xmodelsurfs, 2);
			ReallocateAssetPoolM(physpreset, 2);
			ReallocateAssetPoolM(loaded_sound, 2);
			ReallocateAssetPoolM(sound, 2);
			ReallocateAssetPoolM(weapon, 2);
			ReallocateAssetPoolM(attachment, 2);

			ReallocateAssetPoolM(vertexshader, 2);

			// DB_AddXAssetHeader hook
			Memory(0x44EFEF).jump(DB_AddXAsset);

			// Store Texture Data
			Memory(0x439927).call(StoreTexture);

			// Do not modify loadedsound struct after loading
			Memory(0x0043856E).nop(19);
			Memory(0x00438556).nop(12);

			// Prevent sound data from getting lost
			Memory(0x00438551).jump(MssSound_ReadXFile_stub);

			// Remove check for me_pictureframes (messes up dumping)
			Memory(0x004CC493).set<std::uint8_t>(0xEB);

#ifdef USE_VMPROTECT
			VMProtectEnd();
#endif
		}

		AssetHandler::~AssetHandler()
		{
		}
	}
}
