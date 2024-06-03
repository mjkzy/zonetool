#include "stdafx.hpp"
#include "../Include.hpp"

#include "ParticleSystem.hpp"

namespace ZoneTool::IW5
{
	namespace IW7Converter
	{
		DEFINE_ENUM_FLAG_OPERATORS(IW7::ParticleDataFlags)

		__int64 system_flags;
		__int64 state_flags;
		__int64 emitter_flags;
		__int64 emitterdata_flags;

		int GetModuleNumCurves(IW7::ParticleModuleType moduleType)
		{
			switch (moduleType)
			{
			case IW7::PARTICLE_MODULE_INIT_ATLAS:
			case IW7::PARTICLE_MODULE_EMISSION_GRAPH:
			case IW7::PARTICLE_MODULE_INTENSITY_GRAPH:
			case IW7::PARTICLE_MODULE_PARENT_VELOCITY_GRAPH:
			case IW7::PARTICLE_MODULE_ROTATION_GRAPH:
				return 2;
				break;
			case IW7::PARTICLE_MODULE_INIT_FLARE:
			case IW7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_SPHERE:
				return 4;
				break;
			case IW7::PARTICLE_MODULE_INIT_SPAWN:
				return 1;
				break;
			case IW7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_BOX:
			case IW7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_ELLIPSOID:
			case IW7::PARTICLE_MODULE_POSITION_GRAPH:
			case IW7::PARTICLE_MODULE_ROTATION_GRAPH_3D:
			case IW7::PARTICLE_MODULE_SIZE_GRAPH:
			case IW7::PARTICLE_MODULE_VELOCITY_GRAPH:
				return 6;
				break;
			case IW7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_CYLINDER:
				return 5;
				break;
			case IW7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_MESH:
				return 3;
				break;
			case IW7::PARTICLE_MODULE_COLOR_GRAPH:
				return 8;
				break;
			case IW7::PARTICLE_MODULE_EMISSIVE_GRAPH:
				return 10;
				break;
			default:
				return 0;
				break;
			}
			return 0;
		}

		IW7::PARTICLE_ELEMENT_TYPE convert_elem_type(IW5::FxElemType type)
		{
			switch (type)
			{
			case IW5::FX_ELEM_TYPE_SPRITE_BILLBOARD:
				return IW7::PARTICLE_ELEMENT_TYPE_BILLBOARD_SPRITE;
				break;
			case IW5::FX_ELEM_TYPE_SPRITE_ORIENTED:
				return IW7::PARTICLE_ELEMENT_TYPE_ORIENTED_SPRITE;
				break;
			case IW5::FX_ELEM_TYPE_TAIL:
				return IW7::PARTICLE_ELEMENT_TYPE_TAIL;
				break;
			case IW5::FX_ELEM_TYPE_TRAIL:
				return IW7::PARTICLE_ELEMENT_TYPE_GEO_TRAIL;
				break;
			case IW5::FX_ELEM_TYPE_CLOUD:
				return IW7::PARTICLE_ELEMENT_TYPE_CLOUD;
				break;
			case IW5::FX_ELEM_TYPE_SPARKCLOUD:
				return IW7::PARTICLE_ELEMENT_TYPE_SPARK_CLOUD;
				break;
			case IW5::FX_ELEM_TYPE_SPARKFOUNTAIN:
				__debugbreak();
				break;
			case IW5::FX_ELEM_TYPE_MODEL:
				return IW7::PARTICLE_ELEMENT_TYPE_MODEL;
				break;
			case IW5::FX_ELEM_TYPE_OMNI_LIGHT:
				return IW7::PARTICLE_ELEMENT_TYPE_LIGHT_OMNI;
				break;
			case IW5::FX_ELEM_TYPE_SPOT_LIGHT:
				return IW7::PARTICLE_ELEMENT_TYPE_LIGHT_SPOT;
				break;
			case IW5::FX_ELEM_TYPE_SOUND:
				__debugbreak();
				break;
			case IW5::FX_ELEM_TYPE_DECAL:
				return IW7::PARTICLE_ELEMENT_TYPE_DECAL;
				break;
			case IW5::FX_ELEM_TYPE_RUNNER:
				return IW7::PARTICLE_ELEMENT_TYPE_RUNNER;
				break;
			}

			return IW7::PARTICLE_ELEMENT_TYPE_BILLBOARD_SPRITE;
		}

		void calculate_inv_time_delta(IW7::ParticleCurveDef* curves, unsigned int curves_count)
		{
			for (auto i = 0u; i < curves_count; i++)
			{
				assert(curves[i].numControlPoints > 0);
				assert(curves[i].controlPoints[0].time == 0.0f);
				assert(curves[i].controlPoints[curves[i].numControlPoints - 1].time == 1.0f);

				float prev_time = 0.0f;

				for (auto j = 1u; j < curves[i].numControlPoints; j++)
				{
					curves[i].controlPoints[j].invTimeDelta = 1.0f / (curves[i].controlPoints[j].time - prev_time);
					prev_time = curves[i].controlPoints[j].time;
				}
			}
		}

		// I'm not sure how exactly time is interpreted in FxEffectDefs, so i will assume the visualStateIntervalCount is made in a way that makes sense?
		void calculate_time(FxElemDef* elem, IW7::ParticleCurveDef* curves, unsigned int curves_count)
		{
			float time = 0.0f;
			for (auto i = 0u; i < curves_count; i++)
			{
				assert(curves[i].numControlPoints > 0);
				float step = 1.0f / (curves[i].numControlPoints - 1);
				for (auto j = 0u; j < curves[i].numControlPoints; j++)
				{
					curves[i].controlPoints[j].time = j * step;
				}
			}
		}

		void generate_color_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (!elem->visSamples)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_COLOR_GRAPH;
			auto& moduleData = module.moduleData.colorGraph;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			moduleData.firstCurve = false;
			moduleData.m_modulateColorByAlpha = false;

			for (auto i = 0; i < 8; i++)
			{
				moduleData.m_curves[i].numControlPoints = elem->visStateIntervalCount + 1;
				moduleData.m_curves[i].controlPoints = allocator.allocate<IW7::ParticleCurveControlPointDef>(elem->visStateIntervalCount + 1);

				moduleData.m_curves[i].scale = 1.0f;
			}

			// bgra->rgba
			for (auto i = 0; i < elem->visStateIntervalCount + 1; i++)
			{
				// could be wrong

				moduleData.m_curves[0].controlPoints[i].value = static_cast<int>(elem->visSamples[i].base.color[2]) / 255.0f;
				moduleData.m_curves[1].controlPoints[i].value = static_cast<int>(elem->visSamples[i].base.color[1]) / 255.0f;
				moduleData.m_curves[2].controlPoints[i].value = static_cast<int>(elem->visSamples[i].base.color[0]) / 255.0f;
				moduleData.m_curves[3].controlPoints[i].value = static_cast<int>(elem->visSamples[i].base.color[3]) / 255.0f;

				moduleData.m_curves[4].controlPoints[i].value = static_cast<int>(elem->visSamples[i].amplitude.color[2]) / 255.0f;
				moduleData.m_curves[5].controlPoints[i].value = static_cast<int>(elem->visSamples[i].amplitude.color[1]) / 255.0f;
				moduleData.m_curves[6].controlPoints[i].value = static_cast<int>(elem->visSamples[i].amplitude.color[0]) / 255.0f;
				moduleData.m_curves[7].controlPoints[i].value = static_cast<int>(elem->visSamples[i].amplitude.color[3]) / 255.0f;

				//moduleData.m_curves[4].controlPoints[i].value = moduleData.m_curves[0].controlPoints[i].value + moduleData.m_curves[4].controlPoints[i].value;
				//moduleData.m_curves[5].controlPoints[i].value = moduleData.m_curves[1].controlPoints[i].value + moduleData.m_curves[5].controlPoints[i].value;
				//moduleData.m_curves[6].controlPoints[i].value = moduleData.m_curves[2].controlPoints[i].value + moduleData.m_curves[6].controlPoints[i].value;
				//moduleData.m_curves[7].controlPoints[i].value = moduleData.m_curves[3].controlPoints[i].value + moduleData.m_curves[7].controlPoints[i].value;
			}

			calculate_time(elem, moduleData.m_curves, GetModuleNumCurves(module.moduleType));
			calculate_inv_time_delta(moduleData.m_curves, GetModuleNumCurves(module.moduleType));

			emitterdata_flags |= IW7::USE_COLOR;
			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_HAS_COLOR;

			modules.push_back(module);
		}

		void generate_size_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (!elem->visSamples)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_SIZE_GRAPH;
			auto& moduleData = module.moduleData.sizeGraph;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			moduleData.firstCurve = false;

			for (auto i = 0; i < 6; i++)
			{
				moduleData.m_curves[i].numControlPoints = elem->visStateIntervalCount + 1;
				moduleData.m_curves[i].controlPoints = allocator.allocate<IW7::ParticleCurveControlPointDef>(elem->visStateIntervalCount + 1);
			}

			for (auto i = 0; i < elem->visStateIntervalCount + 1; i++)
			{
				// could be wrong

				moduleData.m_curves[0].scale = std::roundf(elem->visSamples[i].base.size[0]);
				moduleData.m_curves[0].controlPoints[i].value = 1.0f;

				moduleData.m_curves[1].scale = std::roundf(elem->visSamples[i].base.size[0] + elem->visSamples[i].amplitude.size[0]);
				moduleData.m_curves[1].controlPoints[i].value = 1.0f;
				
				moduleData.m_curves[2].scale = 0.0f;
				moduleData.m_curves[2].controlPoints[i].value = 0.0f;

				moduleData.m_curves[3].scale = std::roundf(elem->visSamples[i].base.size[1]);
				moduleData.m_curves[3].controlPoints[i].value = 1.0f;

				moduleData.m_curves[4].scale = std::roundf(elem->visSamples[i].base.size[1] + elem->visSamples[i].amplitude.size[1]);
				moduleData.m_curves[4].controlPoints[i].value = 1.0f;

				moduleData.m_curves[5].scale = 0.0f;
				moduleData.m_curves[5].controlPoints[i].value = 0.0f;
			}

			calculate_time(elem, moduleData.m_curves, GetModuleNumCurves(module.moduleType));
			calculate_inv_time_delta(moduleData.m_curves, GetModuleNumCurves(module.moduleType));

			emitterdata_flags |= IW7::USE_SIZE;

			modules.push_back(module);
		}

		void generate_rotation_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (!elem->visSamples)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_ROTATION_GRAPH;
			auto& moduleData = module.moduleData.rotationGraph;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			module.moduleData.rotationGraph.m_useRotationRate = false; // not sure

			for (auto i = 0; i < 2; i++)
			{
				moduleData.m_curves[i].numControlPoints = elem->visStateIntervalCount + 1;
				moduleData.m_curves[i].controlPoints = allocator.allocate<IW7::ParticleCurveControlPointDef>(elem->visStateIntervalCount + 1);
			}

			for (auto i = 0; i < elem->visStateIntervalCount + 1; i++)
			{
				// could be wrong

				moduleData.m_curves[0].controlPoints[i].value = elem->visSamples[i].base.rotationTotal;
				moduleData.m_curves[1].controlPoints[i].value = elem->visSamples[i].amplitude.rotationTotal;

				moduleData.m_curves[0].scale = elem->visSamples[i].base.scale;
				moduleData.m_curves[1].scale = elem->visSamples[i].amplitude.scale;
			}

			calculate_time(elem, moduleData.m_curves, GetModuleNumCurves(module.moduleType));
			calculate_inv_time_delta(moduleData.m_curves, GetModuleNumCurves(module.moduleType));

			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_HAS_ROTATION_1D_CURVE;

			modules.push_back(module);
		}

		void generate_velocity_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (!elem->velSamples || elem->velIntervalCount < 2)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_VELOCITY_GRAPH;
			auto& moduleData = module.moduleData.velocityGraph;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			moduleData.m_velocityBegin.v[0] = 0.0f;
			moduleData.m_velocityBegin.v[1] = 0.0f;
			moduleData.m_velocityBegin.v[2] = 0.0f;
			moduleData.m_velocityBegin.v[3] = 0.0f;

			moduleData.m_velocityEnd.v[0] = 0.0f;
			moduleData.m_velocityEnd.v[1] = 0.0f;
			moduleData.m_velocityEnd.v[2] = 0.0f;
			moduleData.m_velocityEnd.v[3] = 0.0f;

			for (auto i = 0; i < 6; i++)
			{
				moduleData.m_curves[i].numControlPoints = elem->velIntervalCount + 1;
				moduleData.m_curves[i].controlPoints = allocator.allocate<IW7::ParticleCurveControlPointDef>(elem->velIntervalCount + 1);
			}

			const bool local = (elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0;
			const bool world = (elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_WORLD) != 0;
			assert(local != world);

			moduleData.m_flags |= world ? IW7::PARTICLE_MODULE_FLAG_USE_WORLD_SPACE : 0;

			for (auto i = 0; i < elem->velIntervalCount + 1; i++)
			{
				// could be wrong

				moduleData.m_curves[0].controlPoints[i].value =
					local ? elem->velSamples[i].local.velocity.base[0] : elem->velSamples[i].world.velocity.base[0];
				moduleData.m_curves[1].controlPoints[i].value =
					local ? elem->velSamples[i].local.velocity.base[1] : elem->velSamples[i].world.velocity.base[1];
				moduleData.m_curves[2].controlPoints[i].value =
					local ? elem->velSamples[i].local.velocity.base[2] : elem->velSamples[i].world.velocity.base[2];

				moduleData.m_curves[3].controlPoints[i].value = moduleData.m_curves[0].controlPoints[i].value;
				moduleData.m_curves[4].controlPoints[i].value = moduleData.m_curves[1].controlPoints[i].value;
				moduleData.m_curves[5].controlPoints[i].value = moduleData.m_curves[2].controlPoints[i].value;

				moduleData.m_curves[3].controlPoints[i].value +=
					local ? elem->velSamples[i].local.velocity.amplitude[0] : elem->velSamples[i].world.velocity.amplitude[0];
				moduleData.m_curves[4].controlPoints[i].value +=
					local ? elem->velSamples[i].local.velocity.amplitude[1] : elem->velSamples[i].world.velocity.amplitude[1];
				moduleData.m_curves[5].controlPoints[i].value +=
					local ? elem->velSamples[i].local.velocity.amplitude[2] : elem->velSamples[i].world.velocity.amplitude[2];

				for (auto j = 0; j < 6; j++)
				{
					moduleData.m_curves[j].scale = 1.0f; // IDK
				}
			}

			calculate_time(elem, moduleData.m_curves, GetModuleNumCurves(module.moduleType));
			calculate_inv_time_delta(moduleData.m_curves, GetModuleNumCurves(module.moduleType));

			state_flags |= (elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0 ? IW7::PARTICLE_STATE_DEF_FLAG_HAS_VELOCITY_CURVE_LOCAL : 0;
			state_flags |= (elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_WORLD) != 0 ? IW7::PARTICLE_STATE_DEF_FLAG_HAS_VELOCITY_CURVE_WORLD : 0;

			emitterdata_flags |= IW7::USE_VELOCITY;

			modules.push_back(module);
		}

		void generate_init_spawn_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_SPAWN;
			auto& moduleData = module.moduleData.initSpawn;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			// idk what this is supposed to be

			moduleData.m_curves->scale = 1.0f;
			moduleData.m_curves->numControlPoints = 3;
			moduleData.m_curves->controlPoints = allocator.allocate<IW7::ParticleCurveControlPointDef>(moduleData.m_curves->numControlPoints);
			moduleData.m_curves->controlPoints[0].value = 1.0f;
			moduleData.m_curves->controlPoints[1].value = 1.0f;
			moduleData.m_curves->controlPoints[2].value = 0.0f;

			moduleData.m_curves->controlPoints[0].time = 0.0f;
			moduleData.m_curves->controlPoints[1].time = 0.5f;
			moduleData.m_curves->controlPoints[2].time = 1.0f;

			calculate_time(elem, moduleData.m_curves, GetModuleNumCurves(module.moduleType));
			calculate_inv_time_delta(moduleData.m_curves, GetModuleNumCurves(module.moduleType));

			//emitterdata_flags |= IW7::USE_SPAWN_POS;
			//emitterdata_flags |= IW7::USE_SPAWN_QUAT;

			modules.push_back(module);
		}

		void generate_init_attributes_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			//if (1)
			//{
			//	return;
			//}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_ATTRIBUTES;
			auto& moduleData = module.moduleData.initAttributes;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			moduleData.m_sizeMin.v[0] = 10.0f;
			moduleData.m_sizeMin.v[1] = 10.0f;
			moduleData.m_sizeMin.v[2] = 10.0f;
			moduleData.m_sizeMin.v[3] = 0.0f;
			moduleData.m_sizeMax.v[0] = 10.0f;
			moduleData.m_sizeMax.v[1] = 10.0f;
			moduleData.m_sizeMax.v[2] = 10.0f;
			moduleData.m_sizeMax.v[3] = 0.0f;

			moduleData.m_colorMin.v[0] = 1.0f;
			moduleData.m_colorMin.v[1] = 1.0f;
			moduleData.m_colorMin.v[2] = 1.0f;
			moduleData.m_colorMin.v[3] = 1.0f;
			moduleData.m_colorMax.v[0] = 1.0f;
			moduleData.m_colorMax.v[1] = 1.0f;
			moduleData.m_colorMax.v[2] = 1.0f;
			moduleData.m_colorMax.v[3] = 1.0f;

			moduleData.m_velocityMin.v[0] = 0.0f;
			moduleData.m_velocityMin.v[1] = 0.0f;
			moduleData.m_velocityMin.v[2] = 0.0f;
			moduleData.m_velocityMin.v[3] = 0.0f;
			moduleData.m_velocityMax.v[0] = 0.0f;
			moduleData.m_velocityMax.v[1] = 0.0f;
			moduleData.m_velocityMax.v[2] = 0.0f;
			moduleData.m_velocityMax.v[3] = 0.0f;

			modules.push_back(module);
		}

		void generate_init_rotation_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->initialRotation.base == 0.0f && elem->initialRotation.amplitude == 0.0f)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_ROTATION;
			auto& moduleData = module.moduleData.initRotation;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			// idk
			moduleData.m_rotationAngle.min = elem->initialRotation.base;
			moduleData.m_rotationAngle.max = elem->initialRotation.base + elem->initialRotation.amplitude;

			moduleData.m_rotationRate.min = 0.0f;
			moduleData.m_rotationRate.max = 0.0f;

			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_HAS_ROTATION_1D_INIT;

			emitterdata_flags |= IW7::USE_ROTATION_ANGLE;
			//emitterdata_flags |= IW7::USE_ROTATION_RATE;

			modules.push_back(module);
		}

		void generate_init_atlas_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->atlas.behavior == 0 && elem->atlas.index == 0 && elem->atlas.loopCount == 0)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_ATLAS;
			auto& moduleData = module.moduleData.initAtlas;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			module.moduleData.initAtlas.unk = -1; // ?

			modules.push_back(module);
		}

		void generate_init_decal_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->elemType != FX_ELEM_TYPE_DECAL)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_DECAL;
			auto& moduleData = module.moduleData.initDecal;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			moduleData.m_fadeInTime = static_cast<short>(elem->fadeInRange.base); // idk
			moduleData.m_fadeOutTime = static_cast<short>(elem->fadeOutRange.base); // idk
			moduleData.m_stoppableFadeOutTime = 0;
			moduleData.m_lerpWaitTime = 0;
			moduleData.m_lerpColor.v[0] = 1.0f;
			moduleData.m_lerpColor.v[1] = 1.0f;
			moduleData.m_lerpColor.v[2] = 1.0f;
			moduleData.m_lerpColor.v[3] = 1.0f;

			if (elem->visuals.markArray)
			{
				moduleData.m_linkedAssetList.numAssets = elem->visualCount;
				moduleData.m_linkedAssetList.assetList = allocator.allocate<IW7::ParticleLinkedAssetDef>(moduleData.m_linkedAssetList.numAssets);

				for (int idx = 0; idx < moduleData.m_linkedAssetList.numAssets; idx++)
				{
					if (elem->visuals.markArray[idx].materials[0])
					{
						// mim
						moduleData.m_linkedAssetList.assetList[idx].decal.materials[0] = allocator.manual_allocate<IW7::Material>(8);
						moduleData.m_linkedAssetList.assetList[idx].decal.materials[0]->name = allocator.duplicate_string(
							IW7::replace_material_prefix(elem->visuals.markArray[idx].materials[0]->name));
					}
					if (elem->visuals.markArray[idx].materials[1])
					{
						// wim
						moduleData.m_linkedAssetList.assetList[idx].decal.materials[1] = allocator.manual_allocate<IW7::Material>(8);
						moduleData.m_linkedAssetList.assetList[idx].decal.materials[1]->name = allocator.duplicate_string(
							IW7::replace_material_prefix(elem->visuals.markArray[idx].materials[1]->name));

						// wim autodisplacement
						moduleData.m_linkedAssetList.assetList[idx].decal.materials[2] = allocator.manual_allocate<IW7::Material>(8);
						moduleData.m_linkedAssetList.assetList[idx].decal.materials[2]->name = allocator.duplicate_string(
							IW7::replace_material_prefix(elem->visuals.markArray[idx].materials[1]->name));
					}
				}

				//moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_ASSETS;
			}

			modules.push_back(module);
		}

		void generate_init_model_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->elemType != FX_ELEM_TYPE_MODEL)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_MODEL;
			auto& moduleData = module.moduleData.initModel;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			moduleData.m_usePhysics = (elem->flags & FX_ELEM_USE_MODEL_PHYSICS) != 0;
			moduleData.m_motionBlurHQ = false;

			if (elem->visualCount)
			{
				moduleData.m_linkedAssetList.numAssets = elem->visualCount;
				moduleData.m_linkedAssetList.assetList = allocator.allocate<IW7::ParticleLinkedAssetDef>(moduleData.m_linkedAssetList.numAssets);

				if (elem->visualCount > 1)
				{
					for (int idx = 0; idx < moduleData.m_linkedAssetList.numAssets; idx++)
					{
						moduleData.m_linkedAssetList.assetList[idx].model = reinterpret_cast<IW7::XModel*>(elem->visuals.array[idx].model);
					}
				}
				else
				{
					moduleData.m_linkedAssetList.assetList[0].model = reinterpret_cast<IW7::XModel*>(elem->visuals.instance.model);
				}

				//moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_ASSETS;
			}
			else
			{
				return;
			}

			modules.push_back(module);
		}

		void generate_init_material_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->elemType != FX_ELEM_TYPE_SPRITE_BILLBOARD && elem->elemType != FX_ELEM_TYPE_SPRITE_ORIENTED)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_MATERIAL;
			auto& moduleData = module.moduleData.initMaterial;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			if (elem->visualCount)
			{
				moduleData.m_linkedAssetList.numAssets = elem->visualCount;
				moduleData.m_linkedAssetList.assetList = allocator.allocate<IW7::ParticleLinkedAssetDef>(moduleData.m_linkedAssetList.numAssets);

				if (elem->visualCount > 1)
				{
					for (int idx = 0; idx < moduleData.m_linkedAssetList.numAssets; idx++)
					{
						moduleData.m_linkedAssetList.assetList[idx].material = allocator.manual_allocate<IW7::Material>(8);
						moduleData.m_linkedAssetList.assetList[idx].material->name = allocator.duplicate_string(
							IW7::replace_material_prefix(elem->visuals.array[idx].material->name));
					}
				}
				else
				{
					moduleData.m_linkedAssetList.assetList[0].material = allocator.manual_allocate<IW7::Material>(8);
					moduleData.m_linkedAssetList.assetList[0].material->name = allocator.duplicate_string(
						IW7::replace_material_prefix(elem->visuals.instance.material->name));
				}

				//moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_ASSETS;
			}
			else
			{
				return;
			}

			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_0x400; // dunno
			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_IS_SPRITE;
			system_flags |= IW7::PARTICLE_SYSTEM_DEF_FLAG_HAS_SPRITES;

			modules.push_back(module);
		}

		void generate_init_sprite_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->elemType != FX_ELEM_TYPE_SPRITE_ORIENTED)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_ORIENTED_SPRITE;
			auto& moduleData = module.moduleData.initOrientedSprite;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			moduleData.m_orientationQuat.v[0] = 0.0f;
			moduleData.m_orientationQuat.v[1] = 0.0f;
			moduleData.m_orientationQuat.v[2] = 0.0f;
			moduleData.m_orientationQuat.v[3] = 0.0f;

			modules.push_back(module);
		}

		void generate_init_mirror_texture_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->elemType != FX_ELEM_TYPE_SPRITE_BILLBOARD && elem->elemType != FX_ELEM_TYPE_SPRITE_ORIENTED)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_MIRROR_TEXTURE;
			auto& moduleData = module.moduleData.initMirrorTexture;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			moduleData.m_mirrorHorizontal = IW7::PARTICLE_MIRROR_TEXTURE_TYPE_RANDOM;
			moduleData.m_mirrorVertical = IW7::PARTICLE_MIRROR_TEXTURE_TYPE_RANDOM;

			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_MIRROR_TEXTURE_HORIZONTALLY;
			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_MIRROR_TEXTURE_HORIZONTALLY_RANDOM;
			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_MIRROR_TEXTURE_VERTICALLY;
			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_MIRROR_TEXTURE_VERTICALLY_RANDOM;

			modules.push_back(module);
		}

		void generate_init_spawn_shape_box_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->elemType != FX_ELEM_TYPE_SPRITE_BILLBOARD && elem->elemType != FX_ELEM_TYPE_SPRITE_ORIENTED)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_SPAWN_SHAPE_BOX;
			auto& moduleData = module.moduleData.initSpawnShapeBox;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			// im so done
			//module.moduleData.initSpawnShapeBox.m_dimensionsMin.v[0] = 300.f;
			//module.moduleData.initSpawnShapeBox.m_dimensionsMax.v[0] = 300.f;

			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_HAS_SPAWN_SHAPE;

			modules.push_back(module);
		}

		void generate_init_omni_light_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->elemType != FX_ELEM_TYPE_OMNI_LIGHT)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_LIGHT_OMNI;
			auto& moduleData = module.moduleData.initLightOmni;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			system_flags |= IW7::PARTICLE_SYSTEM_DEF_FLAG_HAS_LIGHTS;
			emitter_flags |= IW7::PARTICLE_EMITTER_DEF_FLAG_HAS_LIGHTS;

			moduleData.m_disableVolumetric = false;
			moduleData.m_tonemappingScaleFactor = 1.0f;
			moduleData.m_intensityIR = 0.0f;
			moduleData.m_exponent = 0;
			// todo?

			if (elem->visualCount)
			{
				moduleData.m_linkedAssetList.numAssets = elem->visualCount;
				moduleData.m_linkedAssetList.assetList = allocator.allocate<IW7::ParticleLinkedAssetDef>(elem->visualCount);

				if (elem->visualCount > 1)
				{
					for (auto i = 0; i < elem->visualCount; i++)
					{
						if (elem->visuals.array[i].lightDef)
						{
							moduleData.m_linkedAssetList.assetList[i].lightDef = allocator.manual_allocate<IW7::GfxLightDef>(8);
							moduleData.m_linkedAssetList.assetList[i].lightDef->name = allocator.duplicate_string(elem->visuals.array[i].lightDef->name);

							//moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_ASSETS;
							moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_LIGHT_DEFS;
						}
					}
				}
				else
				{
					if (elem->visuals.instance.lightDef)
					{
						moduleData.m_linkedAssetList.assetList[0].lightDef = allocator.manual_allocate<IW7::GfxLightDef>(8);
						moduleData.m_linkedAssetList.assetList[0].lightDef->name = allocator.duplicate_string(elem->visuals.instance.lightDef->name);

						//moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_ASSETS;
						moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_LIGHT_DEFS;
					}
				}
			}
			
			modules.push_back(module);
		}

		void generate_init_spot_light_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->elemType != FX_ELEM_TYPE_SPOT_LIGHT)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_LIGHT_SPOT;
			auto& moduleData = module.moduleData.initLightSpot;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			system_flags |= IW7::PARTICLE_SYSTEM_DEF_FLAG_HAS_LIGHTS;
			emitter_flags |= IW7::PARTICLE_EMITTER_DEF_FLAG_HAS_LIGHTS;

			moduleData.m_fovOuter = elem->extended.spotLightDef->fovInnerFraction;
			moduleData.m_fovInner = elem->extended.spotLightDef->fovInnerFraction;
			moduleData.m_bulbRadius = elem->extended.spotLightDef->startRadius;
			moduleData.m_bulbLength = 0.3f;
			moduleData.m_brightness = elem->extended.spotLightDef->brightness;
			moduleData.m_intensityUV = 0.0f;
			moduleData.m_intensityIR = 0.0f;
			moduleData.m_shadowSoftness = 0.5f;
			moduleData.m_shadowBias = 0.4f;
			moduleData.m_shadowArea = 0.01f;
			moduleData.m_shadowNearPlane = 0.0f;
			moduleData.m_toneMappingScaleFactor = 1.0f;
			moduleData.m_exponent = elem->extended.spotLightDef->exponent;

			if (elem->visualCount)
			{
				moduleData.m_linkedAssetList.numAssets = elem->visualCount;
				moduleData.m_linkedAssetList.assetList = allocator.allocate<IW7::ParticleLinkedAssetDef>(elem->visualCount);

				if (elem->visualCount > 1)
				{
					for (auto i = 0; i < elem->visualCount; i++)
					{
						if (elem->visuals.array[i].lightDef)
						{
							moduleData.m_linkedAssetList.assetList[i].lightDef = allocator.manual_allocate<IW7::GfxLightDef>(8);
							moduleData.m_linkedAssetList.assetList[i].lightDef->name = allocator.duplicate_string(elem->visuals.array[i].lightDef->name);

							//moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_ASSETS;
							moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_LIGHT_DEFS;
						}
					}
				}
				else
				{
					if (elem->visuals.instance.lightDef)
					{
						moduleData.m_linkedAssetList.assetList[0].lightDef = allocator.manual_allocate<IW7::GfxLightDef>(8);
						moduleData.m_linkedAssetList.assetList[0].lightDef->name = allocator.duplicate_string(elem->visuals.instance.lightDef->name);

						//moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_ASSETS;
						moduleData.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_LIGHT_DEFS;
					}
				}
			}

			modules.push_back(module);
		}

		void generate_init_geo_trail_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (elem->elemType != FX_ELEM_TYPE_TRAIL)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_GEO_TRAIL;
			auto& moduleData = module.moduleData.initGeoTrail;
			moduleData.type = module.moduleType;
			moduleData.m_flags = 0;

			moduleData.m_numPointsMax = 0xFFFF; // fixme?
			moduleData.m_splitDistance = elem->extended.trailDef->invSplitDist; // correct?
			moduleData.m_splitAngle = elem->extended.trailDef->invSplitArcDist; // correct?
			moduleData.m_centerOffset = 0.0f;
			moduleData.m_numSheets = 0;
			moduleData.m_fadeInDistance = 0.0f;
			moduleData.m_fadeOutDistance = 0.0f;
			moduleData.m_tileDistance = 0.0f;
			moduleData.m_tileOffset.min = 0.0f;
			moduleData.m_tileOffset.max = 0.0f;
			moduleData.m_scrollTime = elem->extended.trailDef->scrollTimeMsec / 1000.f;
			moduleData.m_useLocalVelocity = false;
			moduleData.m_useVerticalTexture = false;
			moduleData.m_cameraFacing = false;
			moduleData.m_fixLeadingEdge = false;
			moduleData.m_clampUVs = false;

			modules.push_back(module);
		}
		
		IW7::ParticleSystemDef* convert(FxEffectDef* asset, allocator& allocator)
		{
			auto* iw7_asset = allocator.allocate<IW7::ParticleSystemDef>();

			iw7_asset->name = asset->name;

			system_flags = 0;

			iw7_asset->numEmitters = asset->elemDefCountLooping + asset->elemDefCountOneShot + asset->elemDefCountEmission;
			iw7_asset->emitterDefs = allocator.allocate<IW7::ParticleEmitterDef>(iw7_asset->numEmitters);
			for (int emitter_index = 0; emitter_index < iw7_asset->numEmitters; emitter_index++)
			{
				const bool looping = emitter_index < asset->elemDefCountLooping;
				const bool one_shot = !looping && emitter_index < asset->elemDefCountOneShot;

				auto* emitter = &iw7_asset->emitterDefs[emitter_index];

				int elem_index = emitter_index;
				auto* elem = &asset->elemDefs[elem_index];

				emitter->flags = 0;
				emitter_flags = 0;

				emitter->m_dataFlags = IW7::USE_NONE;
				emitterdata_flags = 0;

				if (looping)
				{
					emitter->particleSpawnRate.min = elem->spawn.looping.intervalMsec / 1000.0f;
					emitter->particleSpawnRate.max = emitter->particleSpawnRate.min;
				}
				else
				{
					emitter->particleBurstCount.min = elem->spawn.oneShot.count.base;
					emitter->particleBurstCount.max = elem->spawn.oneShot.count.base + elem->spawn.oneShot.count.amplitude;
					emitter->particleCountMax = emitter->particleBurstCount.max;

					emitter_flags |= IW7::PARTICLE_EMITTER_DEF_FLAG_USE_BURST_MODE;
				}

				emitter->particleLife.min = elem->lifeSpanMsec.base / 1000.0f;
				emitter->particleLife.max = elem->lifeSpanMsec.base / 1000.0f + elem->lifeSpanMsec.amplitude / 1000.0f;

				emitter->particleDelay.min = elem->spawnDelayMsec.base / 1000.0f;
				emitter->particleDelay.max = elem->spawnDelayMsec.base / 1000.0f + elem->spawnDelayMsec.amplitude / 1000.0f;

				//emitter->emitterLife.min = elem->lifeSpanMsec.base / 1000.0f;
				//emitter->emitterLife.max = elem->lifeSpanMsec.base / 1000.0f + elem->lifeSpanMsec.amplitude / 1000.0f;

				//emitter->emitterLife.min = elem->spawnDelayMsec.base / 1000.0f;
				//emitter->emitterDelay.max = elem->spawnDelayMsec.base / 1000.0f + elem->spawnDelayMsec.amplitude / 1000.0f;

				emitter->spawnRangeSq.min = 0.0f;
				emitter->spawnRangeSq.max = 2108304.0f;

				emitter->fadeOutMaxDistance = elem->fadeOutRange.base + elem->fadeOutRange.amplitude;

				emitter->spawnFrustumCullRadius = elem->spawnFrustumCullRadius;
				emitter->randomSeed = elem->randomSeed;

				//emitter->particleSpawnShapeRange.min = elem->spawnRange.base;
				//emitter->particleSpawnShapeRange.max = elem->spawnRange.base + elem->spawnRange.amplitude;

				emitter->groupIDs[0] = 0;

				if (emitter->randomSeed)
				{
					emitter->m_dataFlags |= IW7::USE_RANDOM_SEED;
				}

				emitter_flags |= (elem->flags & FX_ELEM_DRAW_PAST_FOG) != 0 ? IW7::PARTICLE_EMITTER_DEF_FLAG_DRAW_PAST_FOG : 0;

				emitter->numStates = 1;
				emitter->stateDefs = allocator.allocate<IW7::ParticleStateDef>(emitter->numStates);

				auto* state = emitter->stateDefs;

				state->elementType = convert_elem_type(elem->elemType);

				state->flags = 0;
				state_flags = 0;

				state_flags |= (elem->flags & FX_ELEM_USE_MODEL_PHYSICS) != 0 ? IW7::PARTICLE_STATE_DEF_FLAG_USE_PHYSICS : 0;
				state_flags |= (elem->flags & FX_ELEM_USE_COLLISION) != 0 ? IW7::PARTICLE_STATE_DEF_FLAG_REQUIRES_WORLD_COLLISION : 0;
				state_flags |= (elem->flags & FX_ELEM_DRAW_WITH_VIEWMODEL) != 0 ? IW7::PARTICLE_STATE_DEF_FLAG_DRAW_WITH_VIEW_MODEL : 0;

				state->moduleGroupDefs = allocator.allocate<IW7::ParticleModuleGroupDef>(IW7::PARTICLE_MODULE_GROUP_COUNT);

				// generate init modules
				{
					std::vector<IW7::ParticleModuleDef> init_modules{};
					generate_init_spawn_module(elem, allocator, init_modules);
					generate_init_attributes_module(elem, allocator, init_modules);
					generate_init_rotation_module(elem, allocator, init_modules);
					generate_init_atlas_module(elem, allocator, init_modules);

					generate_init_material_module(elem, allocator, init_modules);
					generate_init_mirror_texture_module(elem, allocator, init_modules);
					generate_init_decal_module(elem, allocator, init_modules);
					generate_init_model_module(elem, allocator, init_modules);
					generate_init_sprite_module(elem, allocator, init_modules);
					generate_init_spawn_shape_box_module(elem, allocator, init_modules);
					generate_init_omni_light_module(elem, allocator, init_modules);
					generate_init_spot_light_module(elem, allocator, init_modules);
					generate_init_geo_trail_module(elem, allocator, init_modules);

					if (init_modules.size())
					{
						state->moduleGroupDefs[IW7::PARTICLE_MODULE_GROUP_INIT].numModules = init_modules.size();
						state->moduleGroupDefs[IW7::PARTICLE_MODULE_GROUP_INIT].moduleDefs = allocator.allocate<IW7::ParticleModuleDef>(state->moduleGroupDefs[IW7::PARTICLE_MODULE_GROUP_INIT].numModules);
						for (auto i = 0; i < state->moduleGroupDefs[IW7::PARTICLE_MODULE_GROUP_INIT].numModules; i++)
						{
							memcpy(&state->moduleGroupDefs[IW7::PARTICLE_MODULE_GROUP_INIT].moduleDefs[i], &init_modules[i], sizeof(IW7::ParticleModuleDef));
						}
					}
				}

				// generate update modules
				{
					std::vector<IW7::ParticleModuleDef> update_modules{};
					generate_color_module(elem, allocator, update_modules);
					generate_size_module(elem, allocator, update_modules);
					//generate_rotation_module(elem, allocator, update_modules);
					////generate_position_module(elem, allocator, update_modules);
					generate_velocity_module(elem, allocator, update_modules);

					if (update_modules.size())
					{
						state->moduleGroupDefs[IW7::PARTICLE_MODULE_GROUP_UPDATE].numModules = update_modules.size();
						state->moduleGroupDefs[IW7::PARTICLE_MODULE_GROUP_UPDATE].moduleDefs = allocator.allocate<IW7::ParticleModuleDef>(state->moduleGroupDefs[IW7::PARTICLE_MODULE_GROUP_UPDATE].numModules);
						for (auto i = 0; i < state->moduleGroupDefs[IW7::PARTICLE_MODULE_GROUP_UPDATE].numModules; i++)
						{
							memcpy(&state->moduleGroupDefs[IW7::PARTICLE_MODULE_GROUP_UPDATE].moduleDefs[i], &update_modules[i], sizeof(IW7::ParticleModuleDef));
						}
					}
				}

				emitter->m_dataFlags = (IW7::ParticleDataFlags)0x42C80000; //emitter->m_dataFlags |= (IW7::ParticleDataFlags)emitterdata_flags;
				emitter->flags |= emitter_flags;

				state->flags |= state_flags;
			}

			system_flags |= IW7::PARTICLE_SYSTEM_DEF_FLAG_KILL_STOPPED_INFINITE_EFFECTS;
			system_flags |= IW7::PARTICLE_SYSTEM_DEF_FLAG_CANNOT_PRE_ROLL; // not sure what this is

			iw7_asset->flags |= system_flags;

			iw7_asset->version = 15;

			iw7_asset->occlusionOverrideEmitterIndex = -1;

			iw7_asset->phaseOptions = IW7::PARTICLE_PHASE_OPTION_PHASE_NEVER;

			iw7_asset->drawFrustumCullRadius = -1.0f;
			iw7_asset->updateFrustumCullRadius = -1.0f;

			iw7_asset->sunDistance = 100000.000f;

			iw7_asset->editorPosition.v[0] = 0.0f;
			iw7_asset->editorPosition.v[1] = 0.0f;
			iw7_asset->editorPosition.v[2] = 0.0f;
			iw7_asset->editorPosition.v[3] = 0.0f;

			iw7_asset->editorRotation.v[0] = 0.0f;
			iw7_asset->editorRotation.v[1] = 0.0f;
			iw7_asset->editorRotation.v[2] = 0.0f;
			iw7_asset->editorRotation.v[3] = 1.0f;

			return iw7_asset;
		}
	}
}