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
			{"effect_nofog",							"eq_effect_blend_lin_nofog_ndw_nocast"},
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

		std::string prefixes[] =
		{
			"mo",
			"eq",
		};

		std::unordered_map<std::string, std::uint8_t> prefixes_types =
		{
			{"mo", MTL_TYPE_MO},
			{"eq", MTL_TYPE_EQ},
		};

		std::unordered_map<std::string, std::string> prefix_cache;
		std::string replace_material_prefix(const std::string& name, std::string technique)
		{
			if (prefix_cache.contains(name))
			{
				return prefix_cache[name];
			}

			if (!technique.empty() && mapped_techsets.find(technique) != mapped_techsets.end())
			{
				std::string new_tech = mapped_techsets[technique];
				for (auto& prefix : prefixes)
				{
					if (new_tech.starts_with(prefix + "_"))
					{
						auto offset_end = name.find("/");
						if (offset_end == std::string::npos)
						{
							offset_end = 0;
						}

						const auto replacement = prefix + "/";

						std::string replaced = name;
						replaced.replace(0, offset_end, replacement);
						prefix_cache.insert(std::make_pair(name, replaced));
					}
				}
			}

			return name;
		}

		std::uint8_t get_material_type_from_technique(std::string technique)
		{
			if (!technique.empty() && mapped_techsets.find(technique) != mapped_techsets.end())
			{
				std::string new_tech = mapped_techsets[technique];
				for (auto& prefix_pair : prefixes_types)
				{
					const auto& prefix = prefix_pair.first;
					const auto type = prefix_pair.second;

					if (prefix_pair.first.starts_with(prefix + "_"))
					{
						return type;
					}
				}
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