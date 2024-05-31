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

		void generate_color_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (!elem->visSamples)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_COLOR_GRAPH;
			module.moduleData.colorGraph.type = module.moduleType;
			module.moduleData.colorGraph.m_flags = 0;

			module.moduleData.colorGraph.firstCurve = false; // not sure what this means
			module.moduleData.colorGraph.m_modulateColorByAlpha = false; // ^

			for (auto i = 0; i < 8; i++)
			{
				module.moduleData.colorGraph.m_curves[i].numControlPoints = elem->visStateIntervalCount + 1;
				module.moduleData.colorGraph.m_curves[i].controlPoints = allocator.allocate<IW7::ParticleCurveControlPointDef>(elem->visStateIntervalCount + 1);

				module.moduleData.colorGraph.m_curves[i].scale = 1.0f;
			}

			// bgra->rgba
			for (auto i = 0; i < elem->visStateIntervalCount + 1; i++)
			{
				// could be wrong

				module.moduleData.colorGraph.m_curves[0].controlPoints[i].value = static_cast<int>(elem->visSamples[i].base.color[2]) / 255.0f;
				module.moduleData.colorGraph.m_curves[1].controlPoints[i].value = static_cast<int>(elem->visSamples[i].base.color[1]) / 255.0f;
				module.moduleData.colorGraph.m_curves[2].controlPoints[i].value = static_cast<int>(elem->visSamples[i].base.color[0]) / 255.0f;
				module.moduleData.colorGraph.m_curves[3].controlPoints[i].value = static_cast<int>(elem->visSamples[i].base.color[3]) / 255.0f;

				module.moduleData.colorGraph.m_curves[4].controlPoints[i].value = static_cast<int>(elem->visSamples[i].amplitude.color[2]) / 255.0f;
				module.moduleData.colorGraph.m_curves[5].controlPoints[i].value = static_cast<int>(elem->visSamples[i].amplitude.color[1]) / 255.0f;
				module.moduleData.colorGraph.m_curves[6].controlPoints[i].value = static_cast<int>(elem->visSamples[i].amplitude.color[0]) / 255.0f;
				module.moduleData.colorGraph.m_curves[7].controlPoints[i].value = static_cast<int>(elem->visSamples[i].amplitude.color[3]) / 255.0f;

				module.moduleData.colorGraph.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_CURVES;
			}

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
			module.moduleData.sizeGraph.type = module.moduleType;
			module.moduleData.sizeGraph.m_flags = 0;

			module.moduleData.sizeGraph.firstCurve = false; // not sure what this means

			module.moduleData.sizeGraph.m_sizeBegin.v[0] = 0.0f;
			module.moduleData.sizeGraph.m_sizeBegin.v[1] = 0.0f;
			module.moduleData.sizeGraph.m_sizeBegin.v[2] = 0.0f;
			module.moduleData.sizeGraph.m_sizeBegin.v[3] = 0.0f;

			module.moduleData.sizeGraph.m_sizeEnd.v[0] = 0.0f;
			module.moduleData.sizeGraph.m_sizeEnd.v[1] = 0.0f;
			module.moduleData.sizeGraph.m_sizeEnd.v[2] = 0.0f;
			module.moduleData.sizeGraph.m_sizeEnd.v[3] = 0.0f;

			for (auto i = 0; i < 6; i++)
			{
				module.moduleData.sizeGraph.m_curves[i].numControlPoints = elem->visStateIntervalCount + 1;
				module.moduleData.sizeGraph.m_curves[i].controlPoints = allocator.allocate<IW7::ParticleCurveControlPointDef>(elem->visStateIntervalCount + 1);
			}

			for (auto i = 0; i < elem->visStateIntervalCount + 1; i++)
			{
				// could be wrong

				module.moduleData.sizeGraph.m_curves[0].controlPoints[i].value = elem->visSamples[i].base.size[0];
				module.moduleData.sizeGraph.m_curves[1].controlPoints[i].value = elem->visSamples[i].base.size[1];
				module.moduleData.sizeGraph.m_curves[2].controlPoints[i].value = 1.0f;

				module.moduleData.sizeGraph.m_curves[3].controlPoints[i].value = elem->visSamples[i].amplitude.size[0];
				module.moduleData.sizeGraph.m_curves[4].controlPoints[i].value = elem->visSamples[i].amplitude.size[1];
				module.moduleData.sizeGraph.m_curves[5].controlPoints[i].value = 1.0f;

				module.moduleData.sizeGraph.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_CURVES;
			}

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
			module.moduleData.rotationGraph.type = module.moduleType;
			module.moduleData.rotationGraph.m_flags = 0;

			module.moduleData.rotationGraph.m_useRotationRate = false; // not sure

			for (auto i = 0; i < 2; i++)
			{
				module.moduleData.rotationGraph.m_curves[i].numControlPoints = elem->visStateIntervalCount + 1;
				module.moduleData.rotationGraph.m_curves[i].controlPoints = allocator.allocate<IW7::ParticleCurveControlPointDef>(elem->visStateIntervalCount + 1);
			}

			for (auto i = 0; i < elem->visStateIntervalCount + 1; i++)
			{
				// could be wrong

				module.moduleData.rotationGraph.m_curves[0].controlPoints[i].value = elem->visSamples[i].base.rotationTotal;
				module.moduleData.rotationGraph.m_curves[1].controlPoints[i].value = elem->visSamples[i].amplitude.rotationTotal;

				module.moduleData.rotationGraph.m_curves[0].scale = elem->visSamples[i].base.scale;
				module.moduleData.rotationGraph.m_curves[1].scale = elem->visSamples[i].amplitude.scale;

				module.moduleData.rotationGraph.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_CURVES;
			}

			modules.push_back(module);
		}

		void generate_velocity_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			if (!elem->velSamples)
			{
				return;
			}

			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_VELOCITY_GRAPH;
			module.moduleData.velocityGraph.type = module.moduleType;
			module.moduleData.velocityGraph.m_flags = 0;

			module.moduleData.velocityGraph.m_velocityBegin.v[0] = 0.0f;
			module.moduleData.velocityGraph.m_velocityBegin.v[1] = 0.0f;
			module.moduleData.velocityGraph.m_velocityBegin.v[2] = 0.0f;
			module.moduleData.velocityGraph.m_velocityBegin.v[3] = 0.0f;

			module.moduleData.velocityGraph.m_velocityEnd.v[0] = 0.0f;
			module.moduleData.velocityGraph.m_velocityEnd.v[1] = 0.0f;
			module.moduleData.velocityGraph.m_velocityEnd.v[2] = 0.0f;
			module.moduleData.velocityGraph.m_velocityEnd.v[3] = 0.0f;

			for (auto i = 0; i < 6; i++)
			{
				module.moduleData.velocityGraph.m_curves[i].numControlPoints = elem->velIntervalCount + 1;
				module.moduleData.velocityGraph.m_curves[i].controlPoints = allocator.allocate<IW7::ParticleCurveControlPointDef>(elem->velIntervalCount + 1);
			}

			for (auto i = 0; i < elem->velIntervalCount + 1; i++)
			{
				// could be wrong

				module.moduleData.velocityGraph.m_curves[0].controlPoints[i].value = 
					(elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0 ? elem->velSamples[i].local.velocity.base[0] : elem->velSamples[i].world.velocity.base[0];
				module.moduleData.velocityGraph.m_curves[1].controlPoints[i].value =
					(elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0 ? elem->velSamples[i].local.velocity.base[1] : elem->velSamples[i].world.velocity.base[1];
				module.moduleData.velocityGraph.m_curves[2].controlPoints[i].value =
					(elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0 ? elem->velSamples[i].local.velocity.base[2] : elem->velSamples[i].world.velocity.base[2];

				module.moduleData.velocityGraph.m_curves[3].controlPoints[i].value =
					(elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0 ? elem->velSamples[i].local.velocity.amplitude[0] : elem->velSamples[i].world.velocity.amplitude[0];
				module.moduleData.velocityGraph.m_curves[4].controlPoints[i].value =
					(elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0 ? elem->velSamples[i].local.velocity.amplitude[1] : elem->velSamples[i].world.velocity.amplitude[1];
				module.moduleData.velocityGraph.m_curves[5].controlPoints[i].value =
					(elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0 ? elem->velSamples[i].local.velocity.amplitude[2] : elem->velSamples[i].world.velocity.amplitude[2];

				module.moduleData.velocityGraph.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_CURVES;
			}

			emitterdata_flags |= IW7::USE_VELOCITY;

			modules.push_back(module);
		}

		void generate_init_spawn_module(FxElemDef* elem, allocator& allocator, std::vector<IW7::ParticleModuleDef>& modules)
		{
			IW7::ParticleModuleDef module{};
			module.moduleType = IW7::PARTICLE_MODULE_INIT_SPAWN;
			module.moduleData.initSpawn.type = module.moduleType;
			module.moduleData.initSpawn.m_flags = 0;

			module.moduleData.initSpawn.m_curves->numControlPoints = 3; // XYZ ?
			module.moduleData.initSpawn.m_curves->controlPoints = allocator.allocate<IW7::ParticleCurveControlPointDef>(module.moduleData.initSpawn.m_curves->numControlPoints);
			//module.moduleData.initSpawn.m_curves->controlPoints[0].value = elem->spawnOrigin[0].base;
			//module.moduleData.initSpawn.m_curves->controlPoints[1].value = elem->spawnOrigin[1].base;
			//module.moduleData.initSpawn.m_curves->controlPoints[2].value = elem->spawnOrigin[2].base;
			module.moduleData.initSpawn.m_curves->controlPoints[0].value = 1.0f;
			module.moduleData.initSpawn.m_curves->controlPoints[1].value = 1.0f;
			module.moduleData.initSpawn.m_curves->controlPoints[2].value = 1.0f;
			module.moduleData.initSpawn.m_curves->controlPoints[0].time = 1.0f;
			module.moduleData.initSpawn.m_curves->controlPoints[1].time = 1.0f;
			module.moduleData.initSpawn.m_curves->controlPoints[2].time = 1.0f;
			module.moduleData.initSpawn.m_curves->controlPoints[0].invTimeDelta = 1.0f;
			module.moduleData.initSpawn.m_curves->controlPoints[1].invTimeDelta = 1.0f;
			module.moduleData.initSpawn.m_curves->controlPoints[2].invTimeDelta = 1.0f;
			module.moduleData.initSpawn.m_curves->scale = 1.0f;

			module.moduleData.initSpawn.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_CURVES;

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
			module.moduleData.initAttributes.type = module.moduleType;
			module.moduleData.initAttributes.m_flags = 0;

			module.moduleData.initAttributes.m_sizeMin.v[0] = 1.0f;
			module.moduleData.initAttributes.m_sizeMin.v[1] = 1.0f;
			module.moduleData.initAttributes.m_sizeMin.v[2] = 1.0f;
			module.moduleData.initAttributes.m_sizeMin.v[3] = 1.0f;
			module.moduleData.initAttributes.m_sizeMax.v[0] = 1.0f;
			module.moduleData.initAttributes.m_sizeMax.v[1] = 1.0f;
			module.moduleData.initAttributes.m_sizeMax.v[2] = 1.0f;
			module.moduleData.initAttributes.m_sizeMax.v[3] = 1.0f;

			module.moduleData.initAttributes.m_colorMin.v[0] = 0.0f;
			module.moduleData.initAttributes.m_colorMin.v[1] = 0.0f;
			module.moduleData.initAttributes.m_colorMin.v[2] = 0.0f;
			module.moduleData.initAttributes.m_colorMin.v[3] = 0.0f;
			module.moduleData.initAttributes.m_colorMax.v[0] = 1.0f;
			module.moduleData.initAttributes.m_colorMax.v[1] = 1.0f;
			module.moduleData.initAttributes.m_colorMax.v[2] = 1.0f;
			module.moduleData.initAttributes.m_colorMax.v[3] = 1.0f;

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
			module.moduleData.initRotation.type = module.moduleType;
			module.moduleData.initRotation.m_flags = 0;

			// idk
			module.moduleData.initRotation.m_rotationAngle.min = elem->initialRotation.base;
			module.moduleData.initRotation.m_rotationAngle.max = elem->initialRotation.amplitude;

			module.moduleData.initRotation.m_rotationRate.min = 0.0f;
			module.moduleData.initRotation.m_rotationRate.max = 0.0f;

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
			module.moduleData.initAtlas.type = module.moduleType;
			module.moduleData.initAtlas.m_flags = 0;

			//module.moduleData.initAtlas.m_startFrame = elem->atlas.index;
			//module.moduleData.initAtlas.m_loopCount = elem->atlas.loopCount;
			//module.moduleData.initAtlas.m_randomIndex = (elem->atlas.behavior & FX_ATLAS_START_RANDOM) != 0;
			//module.moduleData.initAtlas.m_playOverLife = false; // ?

			module.moduleData.initAtlas.unk = -1;

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
			module.moduleData.initSpawn.type = module.moduleType;
			module.moduleData.initSpawn.m_flags = 0;

			module.moduleData.initDecal.m_fadeInTime = static_cast<short>(elem->fadeInRange.base); // idk
			module.moduleData.initDecal.m_fadeOutTime = static_cast<short>(elem->fadeOutRange.base); // idk
			module.moduleData.initDecal.m_stoppableFadeOutTime = 0;
			module.moduleData.initDecal.m_lerpWaitTime = 0;
			module.moduleData.initDecal.m_lerpColor.v[0] = 1.0f;
			module.moduleData.initDecal.m_lerpColor.v[1] = 1.0f;
			module.moduleData.initDecal.m_lerpColor.v[2] = 1.0f;
			module.moduleData.initDecal.m_lerpColor.v[3] = 1.0f;

			if (elem->visuals.markArray)
			{
				module.moduleData.initDecal.m_linkedAssetList.numAssets = elem->visualCount;
				module.moduleData.initDecal.m_linkedAssetList.assetList = allocator.allocate<IW7::ParticleLinkedAssetDef>(module.moduleData.initDecal.m_linkedAssetList.numAssets);
				for (int idx = 0; idx < module.moduleData.initDecal.m_linkedAssetList.numAssets; idx++)
				{
					if (elem->visuals.markArray[idx].materials[0])
					{
						// mim
						module.moduleData.initDecal.m_linkedAssetList.assetList[idx].decal.materials[0] = allocator.manual_allocate<IW7::Material>(8);
						module.moduleData.initDecal.m_linkedAssetList.assetList[idx].decal.materials[0]->name = allocator.duplicate_string(
							IW7::replace_material_prefix(elem->visuals.markArray[idx].materials[0]->name));
					}
					if (elem->visuals.markArray[idx].materials[1])
					{
						// wim
						module.moduleData.initDecal.m_linkedAssetList.assetList[idx].decal.materials[1] = allocator.manual_allocate<IW7::Material>(8);
						module.moduleData.initDecal.m_linkedAssetList.assetList[idx].decal.materials[1]->name = allocator.duplicate_string(
							IW7::replace_material_prefix(elem->visuals.markArray[idx].materials[1]->name));

						// wim autodisplacement
						module.moduleData.initDecal.m_linkedAssetList.assetList[idx].decal.materials[2] = allocator.manual_allocate<IW7::Material>(8);
						module.moduleData.initDecal.m_linkedAssetList.assetList[idx].decal.materials[2]->name = allocator.duplicate_string(
							IW7::replace_material_prefix(elem->visuals.markArray[idx].materials[1]->name));
					}
				}

				module.moduleData.initDecal.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_ASSETS;
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
			module.moduleData.initModel.type = module.moduleType;
			module.moduleData.initModel.m_flags = 0;

			module.moduleData.initModel.m_usePhysics = (elem->flags & FX_ELEM_USE_MODEL_PHYSICS) != 0;
			module.moduleData.initModel.m_motionBlurHQ = false;

			if (elem->visualCount)
			{
				module.moduleData.initModel.m_linkedAssetList.numAssets = elem->visualCount;
				module.moduleData.initModel.m_linkedAssetList.assetList = allocator.allocate<IW7::ParticleLinkedAssetDef>(module.moduleData.initModel.m_linkedAssetList.numAssets);

				if (elem->visualCount > 1)
				{
					for (int idx = 0; idx < module.moduleData.initModel.m_linkedAssetList.numAssets; idx++)
					{
						module.moduleData.initModel.m_linkedAssetList.assetList[idx].model = reinterpret_cast<IW7::XModel*>(elem->visuals.array[idx].model);
					}
				}
				else
				{
					module.moduleData.initModel.m_linkedAssetList.assetList[0].model = reinterpret_cast<IW7::XModel*>(elem->visuals.instance.model);
				}

				module.moduleData.initModel.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_ASSETS;
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
			module.moduleData.initMaterial.type = module.moduleType;
			module.moduleData.initMaterial.m_flags = 0;

			if (elem->visualCount)
			{
				module.moduleData.initMaterial.m_linkedAssetList.numAssets = elem->visualCount;
				module.moduleData.initMaterial.m_linkedAssetList.assetList = allocator.allocate<IW7::ParticleLinkedAssetDef>(module.moduleData.initMaterial.m_linkedAssetList.numAssets);

				if (elem->visualCount > 1)
				{
					for (int idx = 0; idx < module.moduleData.initMaterial.m_linkedAssetList.numAssets; idx++)
					{
						module.moduleData.initMaterial.m_linkedAssetList.assetList[idx].material = allocator.manual_allocate<IW7::Material>(8);
						module.moduleData.initMaterial.m_linkedAssetList.assetList[idx].material->name = allocator.duplicate_string(
							IW7::replace_material_prefix(elem->visuals.array[idx].material->name));

					}
				}
				else
				{
					module.moduleData.initMaterial.m_linkedAssetList.assetList[0].material = allocator.manual_allocate<IW7::Material>(8);
					module.moduleData.initMaterial.m_linkedAssetList.assetList[0].material->name = allocator.duplicate_string(
						IW7::replace_material_prefix(elem->visuals.instance.material->name));
				}

				module.moduleData.initMaterial.m_flags |= IW7::PARTICLE_MODULE_FLAG_HAS_ASSETS;
			}

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
			module.moduleData.initOrientedSprite.type = module.moduleType;
			module.moduleData.initOrientedSprite.m_flags = 0;

			module.moduleData.initOrientedSprite.m_orientationQuat.v[0] = 0.0f;
			module.moduleData.initOrientedSprite.m_orientationQuat.v[1] = 0.0f;
			module.moduleData.initOrientedSprite.m_orientationQuat.v[2] = 0.0f;
			module.moduleData.initOrientedSprite.m_orientationQuat.v[3] = 0.0f;

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
			module.moduleData.initMirrorTexture.type = module.moduleType;
			module.moduleData.initMirrorTexture.m_flags = 0;

			module.moduleData.initMirrorTexture.m_mirrorHorizontal = IW7::PARTICLE_MIRROR_TEXTURE_TYPE_RANDOM;
			module.moduleData.initMirrorTexture.m_mirrorVertical = IW7::PARTICLE_MIRROR_TEXTURE_TYPE_NONE;

			state_flags |= IW7::PARTICLE_STATE_DEF_FLAG_MIRROR_TEXTURE_HORIZONTALLY_RANDOM;

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
			module.moduleData.initLightOmni.type = module.moduleType;
			module.moduleData.initLightOmni.m_flags = 0;

			system_flags |= IW7::PARTICLE_SYSTEM_DEF_FLAG_HAS_LIGHTS;
			emitter_flags |= IW7::PARTICLE_EMITTER_DEF_FLAG_HAS_LIGHTS;

			module.moduleData.initLightOmni.m_bulbLength = 100.f;
			module.moduleData.initLightOmni.m_bulbRadius = 100.f;

			// todo?
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
			module.moduleData.initLightOmni.type = module.moduleType;
			module.moduleData.initLightOmni.m_flags = 0;

			system_flags |= IW7::PARTICLE_SYSTEM_DEF_FLAG_HAS_LIGHTS;
			emitter_flags |= IW7::PARTICLE_EMITTER_DEF_FLAG_HAS_LIGHTS;

			// todo?
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
			module.moduleData.initGeoTrail.type = module.moduleType;
			module.moduleData.initGeoTrail.m_flags = 0;

			module.moduleData.initGeoTrail.m_numPointsMax = 0xFFFF; // fixme?
			module.moduleData.initGeoTrail.m_splitDistance = elem->extended.trailDef->invSplitDist; // correct?
			module.moduleData.initGeoTrail.m_splitAngle = elem->extended.trailDef->invSplitArcDist; // correct?
			module.moduleData.initGeoTrail.m_centerOffset = 0.0f;
			module.moduleData.initGeoTrail.m_numSheets = 0;
			module.moduleData.initGeoTrail.m_fadeInDistance = 0.0f;
			module.moduleData.initGeoTrail.m_fadeOutDistance = 0.0f;
			module.moduleData.initGeoTrail.m_tileDistance = 0.0f;
			module.moduleData.initGeoTrail.m_tileOffset.min = 0.0f;
			module.moduleData.initGeoTrail.m_tileOffset.max = 0.0f;
			module.moduleData.initGeoTrail.m_scrollTime = elem->extended.trailDef->scrollTimeMsec / 1000.f;
			module.moduleData.initGeoTrail.m_useLocalVelocity = false;
			module.moduleData.initGeoTrail.m_useVerticalTexture = false;
			module.moduleData.initGeoTrail.m_cameraFacing = false;
			module.moduleData.initGeoTrail.m_fixLeadingEdge = false;
			module.moduleData.initGeoTrail.m_clampUVs = false;

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
				auto* emitter = &iw7_asset->emitterDefs[emitter_index];

				int elem_index = emitter_index;
				auto* elem = &asset->elemDefs[elem_index];

				emitter->particleSpawnRate.min = 1.0f;
				emitter->particleSpawnRate.max = 1.0f;
				emitter->particleLife.min = 10.0f;
				emitter->particleLife.max = 10.0f;
				emitter->particleBurstCount.min = 1;
				emitter->particleBurstCount.max = 1;
				emitter->emitByDistanceDensity.min = 0.1;
				emitter->emitByDistanceDensity.max = 0.1;
				emitter->particleSpawnShapeRange.min = 0;
				emitter->particleSpawnShapeRange.max = 0;
				
				//emitter->particleSpawnRate.min = 5.71428585f;
				//emitter->particleSpawnRate.max = 5.71428585f;

				emitter->particleBurstCount.min = elem->spawn.oneShot.count.base;
				emitter->particleBurstCount.max = elem->spawn.oneShot.count.amplitude;

				emitter->particleCountMax = std::max(emitter->particleBurstCount.min, emitter->particleBurstCount.max);

				emitter_flags |= IW7::PARTICLE_EMITTER_DEF_FLAG_USE_BURST_MODE;

				//emitter->particleLife.min = elem->lifeSpanMsec.base / 1000.0f;
				//emitter->particleLife.max = elem->lifeSpanMsec.amplitude / 1000.0f;

				//emitter->particleDelay.min = elem->spawnDelayMsec.base / 1000.0f;
				//emitter->particleDelay.max = elem->spawnDelayMsec.base / 1000.0f;

				//emitter->emitterLife.min = elem->lifeSpanMsec.base / 1000.0f;
				//emitter->emitterLife.max = elem->lifeSpanMsec.amplitude / 1000.0f;

				//emitter->emitterLife.min = elem->spawnDelayMsec.base / 1000.0f;
				//emitter->emitterDelay.max = elem->spawnDelayMsec.amplitude / 1000.0f;

				emitter->spawnRangeSq.min = 0.0f;
				emitter->spawnRangeSq.max = 2108304.0f;

				emitter->fadeOutMaxDistance = std::max(elem->fadeOutRange.base, elem->fadeOutRange.amplitude);

				emitter->spawnFrustumCullRadius = elem->spawnFrustumCullRadius;
				emitter->randomSeed = elem->randomSeed;

				emitter->groupIDs[0] = 0;

				emitter->m_dataFlags = IW7::USE_NONE;
				emitterdata_flags = 0;

				if (emitter->randomSeed)
				{
					emitter->m_dataFlags |= IW7::USE_RANDOM_SEED;
				}

				emitter->flags = 0;
				emitter_flags = 0;

				emitter_flags |= (elem->flags & FX_ELEM_DRAW_PAST_FOG) != 0 ? IW7::PARTICLE_EMITTER_DEF_FLAG_DRAW_PAST_FOG : 0;

				emitter->numStates = 1;
				emitter->stateDefs = allocator.allocate<IW7::ParticleStateDef>(emitter->numStates);

				auto* state = emitter->stateDefs;

				state->elementType = convert_elem_type(elem->elemType);

				state->flags = 0;
				state_flags = 0;

				state_flags |= (elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0 ? IW7::PARTICLE_STATE_DEF_FLAG_HAS_VELOCITY_CURVE_LOCAL : 0;
				state_flags |= (elem->flags & FX_ELEM_HAS_VELOCITY_GRAPH_WORLD) != 0 ? IW7::PARTICLE_STATE_DEF_FLAG_HAS_VELOCITY_CURVE_WORLD : 0;
				state_flags |= (elem->flags & FX_ELEM_USE_MODEL_PHYSICS) != 0 ? IW7::PARTICLE_STATE_DEF_FLAG_USE_PHYSICS : 0;
				state_flags |= (elem->flags & FX_ELEM_USE_COLLISION) != 0 ? IW7::PARTICLE_STATE_DEF_FLAG_REQUIRES_WORLD_COLLISION : 0;

				state->moduleGroupDefs = allocator.allocate<IW7::ParticleModuleGroupDef>(IW7::PARTICLE_MODULE_GROUP_COUNT);

				// generate init modules
				{
					std::vector<IW7::ParticleModuleDef> init_modules{};
					generate_init_spawn_module(elem, allocator, init_modules);
					//generate_init_attributes_module(elem, allocator, init_modules);
					//generate_init_rotation_module(elem, allocator, init_modules);
					generate_init_atlas_module(elem, allocator, init_modules);

					generate_init_decal_module(elem, allocator, init_modules);
					generate_init_model_module(elem, allocator, init_modules);
					generate_init_material_module(elem, allocator, init_modules);
					generate_init_sprite_module(elem, allocator, init_modules);
					generate_init_mirror_texture_module(elem, allocator, init_modules);
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
					//generate_size_module(elem, allocator, update_modules);
					//generate_rotation_module(elem, allocator, update_modules);
					////generate_position_module(elem, allocator, update_modules);
					//generate_velocity_module(elem, allocator, update_modules);

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

				emitter->m_dataFlags = (IW7::ParticleDataFlags)-1; //emitter->m_dataFlags |= (IW7::ParticleDataFlags)emitterdata_flags;
				emitter->flags |= emitter_flags;

				state->flags |= state_flags;
			}

			system_flags |= IW7::PARTICLE_SYSTEM_DEF_FLAG_KILL_STOPPED_INFINITE_EFFECTS;

			iw7_asset->flags |= system_flags;

			iw7_asset->version = 15;

			iw7_asset->occlusionOverrideEmitterIndex = -1;

			iw7_asset->phaseOptions = IW7::PARTICLE_PHASE_OPTION_PHASE_NEVER;

			iw7_asset->drawFrustumCullRadius = 150.000000f;
			iw7_asset->updateFrustumCullRadius = 0.0f;

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