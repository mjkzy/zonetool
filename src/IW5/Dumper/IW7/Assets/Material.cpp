#include "stdafx.hpp"
#include "Material.hpp"

//#include "IW7/Assets/Material.hpp"

namespace ZoneTool
{
	namespace IW7
	{
		std::unordered_map<std::string, std::string> mapped_techsets =
		{
			{"mc_l_sm_r0c0",					"mo_l_sm_replace_i0c0"},
			{"mc_l_sm_r0c0s0",					"mo_l_sm_replace_i0c0s0"},

			{"2d",								"2d"},
		};

		enum MaterialType : std::uint8_t
		{
			MTL_TYPE_DEFAULT = 0x0, // ""
			MTL_TYPE_MO = 23, // "mo"
			MTL_TYPE_MCO = 24, // "mco"
			MTL_TYPE_W = 68, // "w"
			MTL_TYPE_WC = 69, // "wc"
		};

		std::unordered_map<std::string, std::string> material_prefixes[] =
		{
			{{"m", "mo"},
			{"me", "mo"},
			{"mc", "mo"},
			{"w", "w"},
			{"wc", "wc"},
			{"gfx", "ev"}},
		};

		std::string replace_material_prefix(const std::string& name)
		{
			for (auto& prefix_map : material_prefixes)
			{
				for (auto& pair : prefix_map)
				{
					auto prefix = pair.first + "/";
					auto replacement = pair.second + "/";
					
					if (name.starts_with(prefix))
					{
						std::string replaced = name;
						replaced.replace(0, prefix.length(), replacement);
						return replaced;
					}
				}
			}
			return name;
		}

		std::uint8_t get_material_type_from_name(const std::string& name)
		{
			auto prefix_idx = name.find("/");
			if (prefix_idx != std::string::npos && prefix_idx)
			{
				std::string prefix = std::string(name.begin(), name.begin() + prefix_idx);
				if (prefix == "m")
				{
					return IW7::MTL_TYPE_MO;
				}
				else if (prefix == "me")
				{
					return IW7::MTL_TYPE_MO;
				}
				else if (prefix == "mc")
				{
					return IW7::MTL_TYPE_MO;
				}
				else if (prefix == "w")
				{
					return IW7::MTL_TYPE_W;
				}
				else if (prefix == "wc")
				{
					return IW7::MTL_TYPE_WC;
				}
				else if (prefix != "gfx")
				{
					ZONETOOL_WARNING("Unknown material type for prefix \"%s\" (material: %s)", prefix.data(), name.data());
				}
			}
			return H1::MTL_TYPE_DEFAULT;
		}

		IW7::TextureSemantic surf_flags_conversion_table[13]
		{
			IW7::TextureSemantic::TS_2D,
			IW7::TextureSemantic::TS_FUNCTION,
			IW7::TextureSemantic::TS_COLOR_MAP,
			IW7::TextureSemantic::TS_DETAIL_MAP,
			IW7::TextureSemantic::TS_UNUSED_4,
			IW7::TextureSemantic::TS_NORMAL_MAP,
			IW7::TextureSemantic::TS_UNUSED_6,
			IW7::TextureSemantic::TS_UNUSED_7,
			IW7::TextureSemantic::TS_SPECULAR_MAP,
			IW7::TextureSemantic::TS_UNUSED_4,
			IW7::TextureSemantic::TS_UNUSED_4,
			IW7::TextureSemantic::TS_UNUSED_4, // WATER_MAP
			IW7::TextureSemantic::TS_DISPLACEMENT_MAP,
		}; IW5::TextureSemantic;

		std::uint8_t convert_semantic(std::uint8_t from)
		{
			return surf_flags_conversion_table[from];
		}
	}

	namespace IW5::IW7Dumper
	{
	}
}