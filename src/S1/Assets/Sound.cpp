#include "stdafx.hpp"

#include "Sound.hpp"

namespace ZoneTool::S1
{
	namespace snd
	{
#define SND_MAX_VOLUME_GROUPS 179
		std::array<const char*, SND_MAX_VOLUME_GROUPS> volume_mod_groups =
		{
		"default",
		// User interface
		"hud",
		"frontend_sfx",
		"frontend_music",
		"sp_hit_alert_npc",
		// Music
		"music",
		"music2",
		"music_big",
		"music_emitter",
		// Ambience
		"amb_quad",
		"amb_dist",
		"amb_damb",
		"amb_damb_big",
		"amb_emitter",
		"amb_emitter_int",
		"amb_emitter_ext",
		// Physics
		"physics",
		// Characters
		"bodyfall",
		"bodyfall_mech",
		"foley_plr",
		"foley_plr_special",
		"mp_foley_plr",
		"foley_npc",
		"foley_mech",
		"foleymp_npc",
		"wpn_foley_plr",
		"wpn_foley_npc",
		"wpn_foley_mech",
		"footstep_plr",
		"footstep_plr_special",
		"footstep_water_plr",
		"footstep_npc",
		"footstep_mech",
		"mp_footstep_plr",
		"mp_footstep_npc",
		"melee_plr",
		"melee_npc",
		"melee_hit",
		"plr_internal",
		"deaths_door",
		// Dialogue
		"chatter_friendly",
		"chatter_enemy",
		"chatter_atlas",
		"vox_friendly",
		"vox_enemy",
		"dialog",
		"dialog_radio",
		"dialog_critical",
		"dialog_amb",
		// Explosions & Destruction
		"destruct",
		"explosion",
		"explosion_grenade",
		"explosion_flashbang",
		"explosion_rocket",
		"explosion_car",
		"explosion_critical",
		"explosion_debris",
		"impact_critical",
		// Bullet Impacts & Whizbys
		"bullet_impact_geo",
		"bullet_impact_plr",
		"bullet_impact_npc",
		"mp_bullet_impact_geo",
		"mp_bullet_impact_plr",
		"mp_bullet_impact_npc",
		"bullet_whizby",
		"mp_bullet_whizby",
		"bullet_impact_special",
		// Vehicles
		"vehicle_plr",
		"vehicle_plr_special",
		"vehicle_npc",
		"vehicle_npc_special",
		"vehicle_avm_plr",
		"vehicle_avm_npc",
		"wpn_vehicle_plr",
		"wpn_vehicle_npc",
		"vehicle",
		"vehicle_aircraft",
		"vehicle_pdrone",
		"vehicle_drone_swarm",
		"vehicle_amb",
		// Weapons
		"grenadebounce",
		"mp_grenadebounce",
		"shellcasings",
		"mp_shellcasings",
		"wpn_plr",
		"wpn_plr_special",
		"wpn_npc_special",
		"mp_wpn_plr",
		"wpn_npc",
		"mp_wpn_npc",
		"wpn_projectile",
		"mp_wpn_projectile",
		// Weapons Detailed
		"wpn_plr_shot_special",
		"wpn_plr_shot",
		"wpn_plr_mech",
		"wpn_plr_sub",
		"wpn_plr_tail",
		"wpn_npc_shot",
		"wpn_npc_mech",
		"wpn_npc_sub",
		"wpn_npc_tail",
		"wpn_npc_dist",
		// Tactical
		"tactical_plr",
		"tactical_npc",
		"tactical_plr_special",
		"tactical_npc_special",
		// Attack Dogs
		"attack_dog",
		// Special use
		"big_moment",
		"scripted1",
		"scripted2",
		"scripted3",
		"scripted4",
		"scripted5",
		"scripted6",
		"scripted7",
		"scripted8",
		"scripted9",
		"scripted10",
		"fullvolume",
		"mp_pre_event",
		"mp_post_event",
		"mp_splash_notify",
		"mp_hit_marker",
		// Level specific and MP perks
		"mp_perk_quiet",
		// OLD
		"mw3_amb_damb",
		"mw3_amb_emitter",
		"mw3_amb_quad",
		"mw3_bodyfall",
		"mw3_bullet_impact_geo",
		"mw3_bullet_impact_npc",
		"mw3_bullet_impact_plr",
		"mw3_bullet_whizby",
		"mw3_chatter_enemy",
		"mw3_chatter_friendly",
		"mw3_DEPRECATED_default",
		"mw3_DEPRECATED_MAX",
		"mw3_DEPRECATED_na",
		"mw3_destruct",
		"mw3_dialog",
		"mw3_dialog_amb",
		"mw3_dialog_critical",
		"mw3_dialog_radio",
		"mw3_explosion",
		"mw3_explosion_car",
		"mw3_explosion_flashbang",
		"mw3_explosion_grenade",
		"mw3_explosion_rocket",
		"mw3_foley_npc",
		"mw3_footstep_npc",
		"mw3_frontend_sfx",
		"mw3_fullvolume",
		"mw3_grenadebounce",
		"mw3_melee_npc",
		"mw3_mp_bullet_impact_geo",
		"mw3_mp_bullet_impact_npc",
		"mw3_mp_foley_npc",
		"mw3_mp_footstep_npc",
		"mw3_mp_wpn_npc",
		"mw3_music",
		"mw3_physics",
		"mw3_scripted1",
		"mw3_scripted2",
		"mw3_scripted3",
		"mw3_scripted4",
		"mw3_scripted5",
		"mw3_shellcasings",
		"mw3_vehicle",
		"mw3_vehicle_plr",
		"mw3_vehicle_npc",
		"mw3_vox_enemy",
		"mw3_vox_friendly",
		"mw3_wpn_foley_npc",
		"mw3_wpn_foley_plr",
		"mw3_footstep_plr",
		"mw3_wpn_npc",
		"mw3_wpn_plr",
		"mw3_wpn_projectile",
		"mw3_wpn_vehicle_npc",
		"mw3_wpn_vehicle_plr",
		};

#define SND_MAX_DSP_BUSES 30
		std::array<const char*, SND_MAX_DSP_BUSES> dsp_buses =
		{
		"default",
		"weapons",
		"weapons_dist",
		"ambience",
		"emitters",
		"quads",
		"music",
		"foley",
		"explosions",
		"tactical",
		"tactical_special",
		"vehicles",
		"air_vehicles",
		"int_vehicles_1",
		"int_vehicles_2",
		"voices",
		"voices_critical",
		"voices_unfiltered",
		"animals",
		"physics",
		"battlechatter",
		"interface",
		"scripted",
		"scripted1",
		"scripted2",
		"scripted3",
		"scripted4",
		"scripted5",
		"unoccluded",
		"whizbys",
		};

		const char* get_vol_mod_name(short index)
		{
			return volume_mod_groups[index];
		}

		const char* get_dsp_bus_name(char index)
		{
			return dsp_buses[index];
		}

		short get_vol_mod_index_from_name(const char* name)
		{
			for (short i = 0; i < volume_mod_groups.size(); i++)
			{
				const char* vol_mod = volume_mod_groups[i];
				if (!_stricmp(vol_mod, name))
				{
					return i;
				}
			}
			return -1;
		}

		char get_dsp_bus_index_from_name(const char* name)
		{
			for (char i = 0; i < dsp_buses.size(); i++)
			{
				const char* dsp_bus = dsp_buses[i];
				if (!_stricmp(dsp_bus, name))
				{
					return i;
				}
			}
			return -1;
		}
	}
	using namespace snd;

#define SOUND_DUMP_SUBASSET(entry) \
	if (asset->entry) sound[#entry] = asset->entry->name;	\
	else sound[#entry] = nullptr;
#define SOUND_DUMP_STRING(entry) \
	if (asset->entry) sound[#entry] = std::string(asset->entry); \
	else sound[#entry] = nullptr;
#define SOUND_DUMP_FIELD(entry) \
	sound[#entry] = asset->entry;

	void ISound::json_dump_snd_alias(ordered_json& sound, snd_alias_t* asset)
	{
		SOUND_DUMP_STRING(aliasName);
		SOUND_DUMP_STRING(secondaryAliasName);
		SOUND_DUMP_STRING(chainAliasName);
		SOUND_DUMP_STRING(subtitle);
		SOUND_DUMP_STRING(squelchName);

		// soundfile shit
		if (asset->soundFile)
		{
			sound["soundfile"]["type"] = asset->soundFile->type;
			sound["soundfile"]["exists"] = asset->soundFile->exists;

			auto insert_loaded = [&]()
			{
				sound["soundfile"]["name"] = asset->soundFile->u.loadSnd ?
					(asset->soundFile->u.loadSnd->name ? asset->soundFile->u.loadSnd->name : "") :
					"";
			};

			auto insert_streamed = [&]()
			{
				sound["soundfile"]["totalMsec"] = asset->soundFile->u.streamSnd.totalMsec;
				sound["soundfile"]["isLocalized"] = asset->soundFile->u.streamSnd.filename.isLocalized;
				sound["soundfile"]["isStreamed"] = asset->soundFile->u.streamSnd.filename.isStreamed;
				sound["soundfile"]["fileIndex"] = asset->soundFile->u.streamSnd.filename.fileIndex;

				sound["soundfile"]["packed"]["offset"] = 0;
				sound["soundfile"]["packed"]["length"] = 0;
				sound["soundfile"]["raw"]["dir"] = "";
				sound["soundfile"]["raw"]["name"] = "";

				if (asset->soundFile->u.streamSnd.filename.fileIndex)
				{
					sound["soundfile"]["packed"]["offset"] = asset->soundFile->u.streamSnd.filename.info.packed.offset;
					sound["soundfile"]["packed"]["length"] = asset->soundFile->u.streamSnd.filename.info.packed.length;
				}
				else
				{
					sound["soundfile"]["raw"]["dir"] = asset->soundFile->u.streamSnd.filename.info.raw.dir
						? asset->soundFile->u.streamSnd.filename.info.raw.dir
						: "";
					sound["soundfile"]["raw"]["name"] = asset->soundFile->u.streamSnd.filename.info.raw.name
						? asset->soundFile->u.streamSnd.filename.info.raw.name
						: "";
				}
			};

			auto insert_primed = [&]()
			{
				sound["soundfile"]["isLocalized"] = asset->soundFile->u.primedSnd.streamedPart.isLocalized;
				sound["soundfile"]["isStreamed"] = asset->soundFile->u.primedSnd.streamedPart.isStreamed;
				sound["soundfile"]["fileIndex"] = asset->soundFile->u.primedSnd.streamedPart.fileIndex;

				sound["soundfile"]["packed"]["offset"] = 0;
				sound["soundfile"]["packed"]["length"] = 0;
				sound["soundfile"]["raw"]["dir"] = "";
				sound["soundfile"]["raw"]["name"] = "";

				if (asset->soundFile->u.primedSnd.streamedPart.fileIndex)
				{
					sound["soundfile"]["packed"]["offset"] = asset->soundFile->u.primedSnd.streamedPart.info.packed.offset;
					sound["soundfile"]["packed"]["length"] = asset->soundFile->u.primedSnd.streamedPart.info.packed.length;
				}
				else
				{
					sound["soundfile"]["raw"]["dir"] = asset->soundFile->u.primedSnd.streamedPart.info.raw.dir
						? asset->soundFile->u.primedSnd.streamedPart.info.raw.dir
						: "";
					sound["soundfile"]["raw"]["name"] = asset->soundFile->u.primedSnd.streamedPart.info.raw.name
						? asset->soundFile->u.primedSnd.streamedPart.info.raw.name
						: "";
				}

				if (asset->soundFile->u.primedSnd.loadedPart)
				{
					sound["soundfile"]["name"] = asset->soundFile->u.primedSnd.loadedPart ?
						(asset->soundFile->u.primedSnd.loadedPart->name ? asset->soundFile->u.primedSnd.loadedPart->name : "") :
						"";
				}

				sound["soundfile"]["dataOffset"] = asset->soundFile->u.primedSnd.dataOffset;
				sound["soundfile"]["totalSize"] = asset->soundFile->u.primedSnd.totalSize;
				//sound["soundfile"]["primedCrc"] = asset->soundFile->u.primedSnd.primedCrc;
			};

			if (asset->soundFile->type == SAT_LOADED)
			{
				insert_loaded();
			}
			else if (asset->soundFile->type == SAT_STREAMED)
			{
				insert_streamed();
			}
			else if (asset->soundFile->type == SAT_PRIMED)
			{
				insert_primed();
			}
		}

		SOUND_DUMP_FIELD(flags);
		SOUND_DUMP_FIELD(variationType);
		SOUND_DUMP_FIELD(priority);
		sound["dspBus"] = get_dsp_bus_name(asset->dspBusIndex); //SOUND_DUMP_CHAR(dspBusIndex);
		sound["volMod"] = get_vol_mod_name(asset->volModIndex); //SOUND_DUMP_SHORT(volModIndex);
		SOUND_DUMP_FIELD(volMin);
		SOUND_DUMP_FIELD(volMax);
		SOUND_DUMP_FIELD(pitchMin);
		SOUND_DUMP_FIELD(pitchMax);
		SOUND_DUMP_FIELD(distMin);
		SOUND_DUMP_FIELD(distMax);
		SOUND_DUMP_FIELD(velocityMin);
		SOUND_DUMP_FIELD(probability);
		SOUND_DUMP_FIELD(sequence);
		SOUND_DUMP_FIELD(startDelay);

		SOUND_DUMP_FIELD(masterPriority);
		SOUND_DUMP_FIELD(masterPercentage);
		SOUND_DUMP_FIELD(slavePercentage);
		SOUND_DUMP_FIELD(playbackPercentage);

		SOUND_DUMP_FIELD(lfePercentage);
		SOUND_DUMP_FIELD(centerPercentage);

		SOUND_DUMP_FIELD(polyCount);
		SOUND_DUMP_FIELD(playCount);
		SOUND_DUMP_FIELD(polyClass);

		SOUND_DUMP_FIELD(envelopMin);
		SOUND_DUMP_FIELD(envelopMax);

		SOUND_DUMP_FIELD(wetMixOverride);
		SOUND_DUMP_FIELD(focusPercentage);

		SOUND_DUMP_FIELD(smartpanDistance2d);
		SOUND_DUMP_FIELD(smartpanDistance3d);
		SOUND_DUMP_FIELD(smartpanAttenuation3d);
		SOUND_DUMP_FIELD(minSmartpan2dContribution);

		SOUND_DUMP_FIELD(stereo3DAngle);
		SOUND_DUMP_FIELD(stereo3DStart);
		SOUND_DUMP_FIELD(stereo3DEnd);

		SOUND_DUMP_FIELD(threshold);
		SOUND_DUMP_FIELD(lockedLoopTime);

		SOUND_DUMP_SUBASSET(sndContext);
		SOUND_DUMP_SUBASSET(volumeFalloffCurve);
		SOUND_DUMP_SUBASSET(lpfCurve);
		SOUND_DUMP_SUBASSET(reverbSendCurve);

		sound["speakerMap"] = nullptr;
		if (asset->speakerMap)
		{
			json speakerMap;
			speakerMap["name"] = asset->speakerMap->name;
			speakerMap["isDefault"] = asset->speakerMap->isDefault;
			speakerMap["orientation"] = asset->speakerMap->orientation;

			json channelMaps;
			for (char x = 0; x < 2; x++)
			{
				for (char y = 0; y < 2; y++)
				{
					json channelMap;
					channelMap["speakerCount"] = asset->speakerMap->channelMaps[x][y].speakerCount;

					json speakers;
					for (int speaker = 0; speaker < asset->speakerMap->channelMaps[x][y].speakerCount; speaker++)
					{
						json jspeaker;

						jspeaker["speaker"] = asset->speakerMap->channelMaps[x][y].speakers[speaker].speaker;
						jspeaker["numLevels"] = asset->speakerMap->channelMaps[x][y].speakers[speaker].numLevels;
						jspeaker["levels0"] = asset->speakerMap->channelMaps[x][y].speakers[speaker].levels[0];
						jspeaker["levels1"] = asset->speakerMap->channelMaps[x][y].speakers[speaker].levels[1];

						speakers[speaker] = jspeaker;
					}

					channelMap["speakers"] = speakers;

					channelMaps[(x & 0x01) << 1 | y & 0x01] = channelMap;
				}
			}

			speakerMap["channelMaps"] = channelMaps;

			sound["speakerMap"] = speakerMap;
		}

		SOUND_DUMP_FIELD(allowDoppler);
		SOUND_DUMP_SUBASSET(dopplerPreset);
	}

	void ISound::json_dump(snd_alias_list_t* asset)
	{
		const auto path = "sounds\\"s + asset->name + ".json"s;

		ordered_json sound;
		ordered_json aliases;
		ordered_json unknownArray;

		SOUND_DUMP_STRING(aliasName);
		SOUND_DUMP_FIELD(count);

		for (unsigned char i = 0; i < asset->count; i++)
		{
			ordered_json alias;
			json_dump_snd_alias(alias, &asset->head[i]);
			sound["head"][i] = alias;
		}

		for (unsigned char i = 0; i < asset->contextListCount; i++)
		{
			sound["contextList"][i]["aliasOffset"] = asset->contextList[i].aliasOffset;
			sound["contextList"][i]["count"] = asset->contextList[i].count;
		}

		std::string assetstr = sound.dump(4);

		auto file = filesystem::file(path);
		file.open();
		file.write(assetstr.data(), assetstr.size(), 1);
		file.close();
	}

	void ISound::dump(snd_alias_list_t* asset)
	{
		json_dump(asset);
	}
}
