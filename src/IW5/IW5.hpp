// ======================= ZoneTool =======================
// zonetool, a fastfile linker for various
// Call of Duty titles. 
//
// Project: https://github.com/ZoneTool/zonetool
// Author: RektInator (https://github.com/RektInator)
// License: GNU GPL v3.0
// ========================================================
#pragma once

#include <ZoneUtils.hpp>
#include "Functions.hpp"
#include "Structs.hpp"
#include "Patches/FFCompression.hpp"
#include "Patches/AssetHandler.hpp"

#include "X64/X64.hpp"
#include "X64/Utils/IO/filesystem.hpp"
#include "X64/Utils/IO/assetmanager.hpp"

#include "H1/Structs.hpp"
#include "IW6/Structs.hpp"
#include "IW7/Structs.hpp"
#include "S1/Structs.hpp"

using namespace zonetool;

#include "Json.hpp"
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

#include "Assets/ClipMap.hpp"
#include "Assets/ComWorld.hpp"
#include "Assets/FxEffectDef.hpp"
#include "Assets/FxWorld.hpp"
#include "Assets/GfxImage.hpp"
#include "Assets/GfxLightDef.hpp"
#include "Assets/GfxWorld.hpp"
#include "Assets/GlassWorld.hpp"
#include "Assets/LoadedSound.hpp"
#include "Assets/MapEnts.hpp"
#include "Assets/Material.hpp"
#include "Assets/PathData.hpp"
#include "Assets/PhysCollmap.hpp"
#include "Assets/PhysPreset.hpp"
#include "Assets/RawFile.hpp"
#include "Assets/ScriptFile.hpp"
#include "Assets/Sound.hpp"
#include "Assets/SoundCurve.hpp"
#include "Assets/XAnimParts.hpp"
#include "Assets/XModel.hpp"
#include "Assets/XSurface.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		typedef int (*FS_Read_t)(void* buffer, int len, int handle);
		extern FS_Read_t FS_Read;

		extern std::string filesystem_read_big_file(const char* filename);

		class Linker : public ILinker
		{
		private:
			static void MessageLoop();

		public:
			Linker();
			~Linker();

			const char* version() override;
			bool is_used() override;
			void startup() override;
			std::shared_ptr<IZone> alloc_zone(const std::string& zone) override;
			std::shared_ptr<ZoneBuffer> alloc_buffer() override;
			void load_zone(const std::string& name) override;
			void unload_zones() override;
			bool is_valid_asset_type(const std::string& type) override;
			std::int32_t type_to_int(std::string type) override;
			std::string type_to_string(std::int32_t type) override;
            bool supports_building() override;
			bool supports_version(const zone_target_version version) override;

			void dump_zone(const std::string& name, zonetool::dump_target target) override;
			void verify_zone(const std::string& name) override;
		};
	}
}
