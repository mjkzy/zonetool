#include "stdafx.hpp"
#include "Material.hpp"

//#include "H1/Assets/Material.hpp"

#include "IW5/Assets/Material.hpp"

namespace ZoneTool
{
	namespace H1
	{
		namespace
		{
			std::string get_h1_techset(std::string name, std::string matname, bool* result)
			{
				if (mapped_techsets.find(name) == mapped_techsets.end())
				{
					ZONETOOL_ERROR("Could not find mapped H1 techset for IW4 techset \"%s\" (material: %s)", name.data(), matname.data());
					*result = false;
					return "2d";
				}
				*result = true;
				return mapped_techsets[name];
			}

			std::unordered_map<std::uint8_t, std::uint8_t> mapped_sortkeys =
			{
				// TODO: check if these are right for IW4 -> H1
				{0, 1},		// Opaque ambient
				{1, 2},		// Opaque
				{2, 3},		// Sky
				{3, 4},		// Skybox sun/moon
				{4, 5},		// Clouds  NOT SURE
				{5, 6},		// Horizon NOT SURE
				{6, 7},		// Decal bottom 1
				{7, 8},		// Decal bottom 2
				{8, 9},		// Decal bottom 3
				{9, 10},	// Decal static
				{10, 11},	// Decal mid 1
				{11, 12},	// Decal mid 2
				{12, 13},	// Decal mid 3
				{13, 14},	// Weapon Impact
				{14, 15},	// Decal top 1
				{15, 16},	// Decal top 2
				{16, 17},	// Decal top 3
				{17, 18},	// Shadow?
				//{18, },	// ?
				//{19, },	// ?
				{20, 22},	// Water Ocean?
				//{21, },	// ?
				//{22, },	// ?
				//{23, },	// ?
				{24, 26},	// Window inside
				{25, 27},	// Window outside
				{26, 28},	// Cloud 1
				{27, 29},	// Cloud 2
				{28, 30},	// Cloud 3
				{29, 31},	// Blend 1
				//{30, 32},	// Blend 2
				//{31, 33},	// Blend 3
				//{32, },	// ?
				{33, 22},	// Water?
				{34, 38},	// Cloud dust?
				//{35, 35},	// Opaque
				//{36, },	// ?
				//{37, },	// ?
				//{38, },	// ?
				//{39, },	// ?
				//{40, },	// ?
				//{41, },	// ?
				//{42, },	// ?
				{43, 48},	// Distortion
				{44, 49},	// Distortion heavy
				{45, 50},	// ?
				{46, 51},	// ?
				{47, 52},	// Effect 1
				{48, 53},	// Effect 2
				{49, 54},	// Effect 3
				{50, 55},	// ?
				{51, 56},	// ?
				//{52, 57},	// ?
				{53, 36},	// Viewmodel Effect
			};

			std::unordered_map<std::string, std::uint8_t> mapped_sortkeys_by_techset =
			{
				// TODO: check if these are right for IW4 -> H1
				{"2d", 60},
				{"mc_shadowcaster_atest", 38},
				{"wc_shadowcaster", 38},
			};

			std::uint8_t get_h1_sortkey(std::uint8_t sortkey, std::string matname, std::string h1_techset)
			{
				if (mapped_sortkeys_by_techset.find(h1_techset) != mapped_sortkeys_by_techset.end())
				{
					return mapped_sortkeys_by_techset[h1_techset];
				}

				if (mapped_sortkeys.contains(sortkey))
				{
					return mapped_sortkeys[sortkey];
				}

				ZONETOOL_ERROR("Could not find mapped H1 sortkey for sortkey: %d (material: %s)", sortkey, matname.data());

				return sortkey;
			}

			std::unordered_map<std::uint8_t, std::uint8_t> mapped_camera_regions =
			{
				{IW4::CAMERA_REGION_LIT_OPAQUE, H1::CAMERA_REGION_LIT_OPAQUE},
				{IW4::CAMERA_REGION_LIT_TRANS, H1::CAMERA_REGION_LIT_TRANS},
				{IW4::CAMERA_REGION_EMISSIVE, H1::CAMERA_REGION_EMISSIVE},
				{IW4::CAMERA_REGION_DEPTH_HACK, H1::CAMERA_REGION_DEPTH_HACK},
				{IW4::CAMERA_REGION_LIGHT_MAP_OPAQUE, H1::CAMERA_REGION_LIT_OPAQUE},
				{IW4::CAMERA_REGION_NONE, H1::CAMERA_REGION_NONE},
			};

			std::unordered_map<std::string, std::uint8_t> mapped_camera_regions_by_techset =
			{
				{"2d", H1::CAMERA_REGION_NONE},
				{"mc_shadowcaster_atest", H1::CAMERA_REGION_NONE},
				{"wc_shadowcaster", H1::CAMERA_REGION_NONE},
			};

			std::uint8_t get_h1_camera_region(std::uint8_t camera_region, std::string matname, std::string h1_techset)
			{
				if (matname.find("_dec") != std::string::npos)
				{
					//return H1::CAMERA_REGION_LIT_DECAL;
				}

				if (mapped_camera_regions_by_techset.find(h1_techset) != mapped_camera_regions_by_techset.end())
				{
					return mapped_camera_regions_by_techset[h1_techset];
				}

				if (mapped_camera_regions.contains(camera_region))
				{
					return mapped_camera_regions[camera_region];
				}

				ZONETOOL_ERROR("Could not find mapped H1 camera region for camera region: %d (material: %s)", camera_region, matname.data());

				return camera_region;
			}

			std::uint8_t get_material_type_from_name(const std::string& name)
			{
				auto prefix_idx = name.find("/");
				if (prefix_idx != std::string::npos && prefix_idx)
				{
					std::string prefix = std::string(name.begin(), name.begin() + prefix_idx);
					if (prefix == "m")
					{
						return H1::MTL_TYPE_MODEL;
					}
					else if (prefix == "me")
					{
						return H1::MTL_TYPE_MODEL_GREY;
					}
					else if (prefix == "mc")
					{
						return H1::MTL_TYPE_MODEL_VERTCOL;
					}
					else if (prefix == "w")
					{
						return H1::MTL_TYPE_WORLD;
					}
					else if (prefix == "wc")
					{
						return H1::MTL_TYPE_WORLD_VERTCOL;
					}
					else if (prefix != "gfx")
					{
						ZONETOOL_WARNING("Unknown material type for prefix \"%s\" (material: %s)", prefix.data(), name.data());
					}
				}
				return H1::MTL_TYPE_DEFAULT;
			}
		}
	}

	namespace
	{
		std::string clean_name(const std::string& name)
		{
			auto new_name = name;

			for (auto i = 0u; i < name.size(); i++)
			{
				switch (new_name[i])
				{
				case '*':
					new_name[i] = '_';
					break;
				}
			}

			return new_name;
		}

#define MATERIAL_DUMP_STRING(entry) \
	matdata[#entry] = std::string(asset->entry);

#define MATERIAL_DUMP(entry) \
	matdata[#entry] = asset->entry;

#define MATERIAL_DUMP_INFO(entry) \
	matdata[#entry] = asset->info.entry;
	}

	namespace IW4::H1Dumper
	{
		void dump(Material* asset)
		{
			if (asset)
			{
				auto c_name = clean_name(asset->name);

				const auto path = "materials\\"s + c_name + ".json"s;
				auto file = zonetool::filesystem::file(path);

				ordered_json matdata;

				MATERIAL_DUMP_STRING(name);

				std::string techset;
				std::string h1_techset;
				if (asset->techniqueSet)
				{
					techset = asset->techniqueSet->name;
					if (techset.starts_with("iw3/"))
					{
						techset.erase(0, 4);
					}

					if (techset.ends_with("_sat"))
					{
						auto idx = techset.find("_sat");
						techset.erase(idx, 4);
					}

					if (techset.ends_with(".3x")) // iw3xport
					{
						auto idx = techset.find(".3x");
						techset.erase(idx, 3);
					}

					bool result = false;
					h1_techset = H1::get_h1_techset(techset, asset->name, &result);
					/*if (!result)
					{
						ZONETOOL_ERROR("Not dumping material \"%s\"", asset->name);
						return;
					}*/
					matdata["techniqueSet->name"] = h1_techset;
					matdata["techniqueSet->og_name"] = techset;
				}

				matdata["gameFlags"] = asset->gameFlags; // convert
				matdata["sortKey"] = H1::get_h1_sortkey(asset->sortKey, asset->name, h1_techset); // convert
				matdata["renderFlags"] = 0; // idk

				matdata["textureAtlasRowCount"] = asset->animationX;
				matdata["textureAtlasColumnCount"] = asset->animationY;
				matdata["textureAtlasFrameBlend"] = 0;
				matdata["textureAtlasAsArray"] = 0;

				matdata["surfaceTypeBits"] = asset->surfaceTypeBits; // convert
				// hashIndex;

				//matdata["stateFlags"] = asset->stateFlags; // convert
				matdata["cameraRegion"] = H1::get_h1_camera_region(asset->cameraRegion, asset->name, h1_techset);
				matdata["materialType"] = H1::get_material_type_from_name(asset->name);
				matdata["assetFlags"] = H1::MTL_ASSETFLAG_NONE;

				ordered_json constant_table = json::array();
				for (int i = 0; i < asset->constantCount && techset != "2d"; i++)
				{
					ordered_json table;
					std::string constant_name = asset->constantTable[i].name;
					const auto constant_hash = asset->constantTable[i].nameHash;

					if (constant_name.size() > 12)
					{
						constant_name.resize(12);
					}

					if (h1_techset.find("_ct_") == std::string::npos || h1_techset.find("ct0") == std::string::npos)
					{
						if (constant_name == "colorTint" &&
							asset->constantTable[i].literal[0] == 1.0f &&
							asset->constantTable[i].literal[1] == 1.0f &&
							asset->constantTable[i].literal[2] == 1.0f &&
							asset->constantTable[i].literal[3] == 1.0f)
						{
							continue;
						}
					}

					if (constant_hash == 148072969) // detailScale
					{
						const auto s1 = h1_techset.find("d0");
						if (s1 != std::string::npos)
						{
							const auto s2 = h1_techset.find("sd0");
							if (s2 - 1 == s1)
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}

					table["name"] = constant_name.data();
					table["nameHash"] = asset->constantTable[i].nameHash;

					nlohmann::json literal_entry;
					literal_entry[0] = asset->constantTable[i].literal[0];
					literal_entry[1] = asset->constantTable[i].literal[1];
					literal_entry[2] = asset->constantTable[i].literal[2];
					literal_entry[3] = asset->constantTable[i].literal[3];
					table["literal"] = literal_entry;

					constant_table[constant_table.size()] = table;
				}

#define CONSTANT_TABLE_ADD_IF_NOT_FOUND(CONST_NAME, CONST_HASH, LITERAL_1, LITERAL_2, LITERAL_3, LITERAL_4) \
				bool has_const = false; \
				std::size_t insert_position = constant_table.size(); \
				for (std::size_t i = 0; i < constant_table.size(); i++) \
				{ \
					if (constant_table[i]["nameHash"].get<std::size_t>() == CONST_HASH) \
					{ \
						has_const = true; \
						break; \
					} \
					if (constant_table[i]["nameHash"].get<std::size_t>() > CONST_HASH) \
					{ \
						insert_position = i; \
						break; \
					} \
				} \
				if (!has_const) \
				{ \
					ordered_json table; \
					table["name"] = CONST_NAME; \
					table["nameHash"] = CONST_HASH; \
					nlohmann::json literal_entry; \
					literal_entry[0] = LITERAL_1; \
					literal_entry[1] = LITERAL_2; \
					literal_entry[2] = LITERAL_3; \
					literal_entry[3] = LITERAL_4; \
					table["literal"] = literal_entry; \
					constant_table.insert(constant_table.begin() + insert_position, table); \
				}

				if (h1_techset.find("sd0") != std::string::npos)
				{
					CONSTANT_TABLE_ADD_IF_NOT_FOUND("envMapParms", 1033475292, 0.0f, 0.0f, 0.0f, 0.0f);
				}

				if (h1_techset.find("_flag_") != std::string::npos)
				{
					CONSTANT_TABLE_ADD_IF_NOT_FOUND("flagParms", 2292903761, 1.0f, 0.0f, 0.0f, 0.0f);
				}

				if (h1_techset.find("_cso_") != std::string::npos)
				{
					CONSTANT_TABLE_ADD_IF_NOT_FOUND("hdrColorParm", 3379990750, 1.0f, 0.0f, 0.0f, 0.0f);
				}

				if (h1_techset.find("shadowcaster") != std::string::npos)
				{
					CONSTANT_TABLE_ADD_IF_NOT_FOUND("uvScale", 3809220315, 1.0f, 1.0f, 0.0f, 0.0f);
				}

				matdata["constantTable"] = constant_table;

				ordered_json material_images = json::array();
				for (auto i = 0; i < asset->numMaps; i++)
				{
					ordered_json image;
					if (asset->maps[i].semantic == 11)
					{
						auto* water = reinterpret_cast<water_t*>(asset->maps[i].image);
						if (water->image && water->image->name)
						{
							image["image"] = water->image->name;
						}
						else
						{
							image["image"] = "";
						}
						//image["waterinfo"] = nullptr;
					}
					else
					{
						if (asset->maps[i].image && asset->maps[i].image->name)
						{
							image["image"] = asset->maps[i].image->name;
						}
						else
						{
							ZONETOOL_WARNING("material %s has an empty image, assigning default...", asset->name);
							image["image"] = "default";
						}
					}

					image["semantic"] = asset->maps[i].semantic == 11 ? 2 : asset->maps[i].semantic; // convert? ( should be the same )
					image["samplerState"] = asset->maps[i].sampleState == 11 ? 19 : asset->maps[i].sampleState; // convert? ( should be fine )
					image["lastCharacter"] = asset->maps[i].secondLastCharacter;
					image["firstCharacter"] = asset->maps[i].firstCharacter;
					image["typeHash"] = asset->maps[i].typeHash;

					// add image data to material
					material_images.push_back(image);
				}

#define IMAGE_ADD_IF_NOT_FOUND(IMAGE, SEMANTIC, SAMPLER_STATE, LAST_CHARACTER, FIRST_CHARACTER, HASH) \
				bool has_image = false; \
				std::size_t insert_position = material_images.size(); \
				for (std::size_t i = 0; i < material_images.size(); i++) \
				{ \
					if (material_images[i]["typeHash"].get<std::size_t>() == HASH) \
					{ \
						has_image = true; \
						break; \
					} \
					if (material_images[i]["typeHash"].get<std::size_t>() > HASH) \
					{ \
						insert_position = i; \
						break; \
					} \
				} \
				if (!has_image) \
				{ \
					ordered_json image; \
					image["image"] = IMAGE; \
					image["semantic"] = SEMANTIC; \
					image["samplerState"] = SAMPLER_STATE; \
					image["lastCharacter"] = LAST_CHARACTER; \
					image["firstCharacter"] = FIRST_CHARACTER; \
					image["typeHash"] = HASH; \
					material_images.insert(material_images.begin() + insert_position, image); \
				}

				if (h1_techset.find("n0") != std::string::npos)
				{
					IMAGE_ADD_IF_NOT_FOUND("$identitynormalmap", 5, 1, 112, 110, 1507003663);
				}

				if (h1_techset.find("sd0") != std::string::npos)
				{
					IMAGE_ADD_IF_NOT_FOUND("$identitynormalmap", 8, 19, 112, 115, 887934131);
				}

				if (h1_techset.find("_lmpb_") != std::string::npos || h1_techset.find("_flag_") != std::string::npos)
				{
					IMAGE_ADD_IF_NOT_FOUND("~envbrdflut_ggx_16-rg", 15, 226, 116, 101, 3447584578);
				}

				matdata["textureTable"] = material_images;

				auto str = matdata.dump(4, ' ', true, nlohmann::detail::error_handler_t::replace);

				matdata.clear();

				file.open("wb");
				file.write(str);
				file.close();
			}
		}
	}
}