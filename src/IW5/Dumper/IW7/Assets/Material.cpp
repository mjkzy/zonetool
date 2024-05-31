#include "stdafx.hpp"
#include "Material.hpp"

//#include "IW7/Assets/Material.hpp"

namespace ZoneTool
{
	namespace IW7
	{
		std::unordered_map<std::string, std::string> mapped_techsets =
		{
			{"mc_l_sm_r0c0",							"mo_l_sm_replace_i0c0"},
			{"mc_l_sm_r0c0s0",							"mo_l_sm_replace_i0c0s0"},
			{"mc_l_sm_r0c0n0s0",						"mo_l_sm_replace_i0c0s0n0"},

			{"2d",										"2d"},

			{"effect",									"eq_effect_blend_lin_ndw_nocast"},
			{"effect_zfeather_add",						"eq_effect_zfeather_add_lin_ct_ndw_nocast"},
			{"effect_zfeather_add_nofog_eyeoffset",		"eq_effect_zfeather_add_lin_nofog_eyeoffset_ndw_nocast"},
			{"effect_zfeather_add_nofog",				"eq_effect_zfeather_add_lin_nofog_ndw_nocast"},

			{"distortion_scale",						"eq_distortion_scale"},
			{"distortion_scale_zfeather",				"eq_distortion_scale_zfeather"},
		};

		enum MaterialType : std::uint8_t
		{
			MTL_TYPE_DEFAULT = 0x0, // ""
			MTL_TYPE_M = 1, // "m"
			MTL_TYPE_MO = 23, // "mo"
			MTL_TYPE_MCO = 24, // "mco"
			MTL_TYPE_W = 68, // "w"
			MTL_TYPE_WC = 69, // "wc"
			MTL_TYPE_EQ = 73, // "eq"
		};

		std::unordered_map<std::string, std::string> material_prefixes[] =
		{
			{{"m", "mo"},
			{"me", "mo"},
			{"mc", "mo"},
			{"w", "w"},
			{"wc", "wc"}}
		};

		std::unordered_map<std::string, std::string> prefix_cache;
		std::string replace_material_prefix(const std::string& name, std::string opt)
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
			if (prefix_cache.contains(name) || opt.size() && (opt.find("effect") != std::string::npos || opt.find("dist") != std::string::npos))
			{
				auto replaced = "eq"s + "/"s + name;
				prefix_cache.insert(std::make_pair(name, replaced));
				return replaced;
			}
			return name;
		}

		std::uint8_t get_material_type_from_name(const std::string& name, std::string opt)
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
				else
				{
					ZONETOOL_WARNING("Unknown material type for prefix \"%s\" (material: %s)", prefix.data(), name.data());
				}
			}

			if (opt.size() && (opt.find("effect") != std::string::npos || opt.find("dist") != std::string::npos))
			{
				return IW7::MTL_TYPE_EQ;
			}

			return IW7::MTL_TYPE_DEFAULT;
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