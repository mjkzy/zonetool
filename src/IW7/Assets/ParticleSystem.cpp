#include "stdafx.hpp"

namespace ZoneTool::IW7
{
	ParticleModuleDef* parent_module;

	void dump_particle_linked_asset_def(ParticleLinkedAssetDef* asset, assetmanager::dumper& dump)
	{
		switch (parent_module->moduleType)
		{
		case PARTICLE_MODULE_INIT_MATERIAL:
			dump.dump_asset(asset->material);
			break;
		case PARTICLE_MODULE_INIT_MODEL:
		case PARTICLE_MODULE_INIT_SPAWN_SHAPE_MESH:
			dump.dump_asset(asset->model);
			break;
		case PARTICLE_MODULE_PHYSICS_LIGHT:
			dump.dump_asset(asset->physicsFXData.physicsFXPipeline);
			dump.dump_asset(asset->physicsFXData.physicsFXShape);
			break;
		case PARTICLE_MODULE_INIT_PARTICLE_SIM:
			dump.dump_asset(asset->particleSim);
			break;
		case PARTICLE_MODULE_INIT_DECAL:
			dump.dump_asset(asset->decal.materials[0]);
			dump.dump_asset(asset->decal.materials[1]);
			dump.dump_asset(asset->decal.materials[2]);
			break;
		case PARTICLE_MODULE_INIT_SOUND:
			dump.dump_string(asset->sound);
			break;
		case PARTICLE_MODULE_INIT_VECTOR_FIELD:
			dump.dump_asset(asset->vectorField);
			break;
		case PARTICLE_MODULE_INIT_LIGHT_OMNI:
		case PARTICLE_MODULE_INIT_LIGHT_SPOT:
			dump.dump_asset(asset->lightDef);
			break;
		case PARTICLE_MODULE_INIT_BEAM:
		case PARTICLE_MODULE_INIT_RUNNER:

		case PARTICLE_MODULE_TEST_AGE:
		case PARTICLE_MODULE_TEST_BIRTH:
		case PARTICLE_MODULE_TEST_DEATH:
		case PARTICLE_MODULE_TEST_GRAVITY:
		case PARTICLE_MODULE_TEST_IMPACT:
		case PARTICLE_MODULE_TEST_POS:
			dump.dump_asset(asset->particleSystem);
			break;
		default:
			__debugbreak();
			break;
		}
	}

	void dump_particle_linked_asset_list(ParticleLinkedAssetListDef& asset, assetmanager::dumper& dump)
	{
		dump.dump_array(asset.assetList, asset.numAssets);
		for (auto i = 0; i < asset.numAssets; i++)
		{
			dump_particle_linked_asset_def(&asset.assetList[i], dump);
		}
	}

	void dump_module(ParticleModuleDef* asset, assetmanager::dumper& dump)
	{
		parent_module = asset;

		switch (asset->moduleType)
		{
		case PARTICLE_MODULE_INIT_ATLAS:
			break;
		case PARTICLE_MODULE_INIT_ATTRIBUTES:
			break;
		case PARTICLE_MODULE_INIT_BEAM:
			dump_particle_linked_asset_list(asset->moduleData.initBeam.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_INIT_CAMERA_OFFSET:
			break;
		case PARTICLE_MODULE_INIT_CLOUD:
			for (auto i = 0; i < 2; i++)
			{
				dump.dump_array(asset->moduleData.initCloud.curves[i].controlPoints, asset->moduleData.initCloud.curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_INIT_DECAL:
			dump_particle_linked_asset_list(asset->moduleData.initDecal.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_INIT_FLARE:
			for (auto i = 0; i < 4; i++)
			{
				dump.dump_array(asset->moduleData.initFlare.m_curves[i].controlPoints, asset->moduleData.initFlare.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_INIT_GEO_TRAIL:
			break;
		case PARTICLE_MODULE_INIT_LIGHT_OMNI:
			dump_particle_linked_asset_list(asset->moduleData.initLightOmni.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_INIT_LIGHT_SPOT:
			dump_particle_linked_asset_list(asset->moduleData.initLightSpot.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_INIT_MATERIAL:
			dump_particle_linked_asset_list(asset->moduleData.initMaterial.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_INIT_MIRROR_TEXTURE:
			break;
		case PARTICLE_MODULE_INIT_MODEL:
			dump_particle_linked_asset_list(asset->moduleData.initModel.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_INIT_OCCLUSION_QUERY:
			break;
		case PARTICLE_MODULE_INIT_ORIENTED_SPRITE:
			break;
		case PARTICLE_MODULE_INIT_PARTICLE_SIM:
			dump_particle_linked_asset_list(asset->moduleData.initParticleSim.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_INIT_PLAYER_FACING:
			break;
		case PARTICLE_MODULE_INIT_RELATIVE_VELOCITY:
			break;
		case PARTICLE_MODULE_INIT_ROTATION:
			break;
		case PARTICLE_MODULE_INIT_ROTATION_3D:
			break;
		case PARTICLE_MODULE_INIT_RUNNER:
			dump_particle_linked_asset_list(asset->moduleData.initRunner.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_INIT_SOUND:
			dump_particle_linked_asset_list(asset->moduleData.initSound.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_INIT_SPAWN:
			for (auto i = 0; i < 1; i++)
			{
				dump.dump_array(asset->moduleData.initSpawn.m_curves[i].controlPoints, asset->moduleData.initSpawn.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_INIT_SPAWN_SHAPE_BOX:
			break;
		case PARTICLE_MODULE_INIT_SPAWN_SHAPE_CYLINDER:
			break;
		case PARTICLE_MODULE_INIT_SPAWN_SHAPE_ELLIPSOID:
			break;
		case PARTICLE_MODULE_INIT_SPAWN_SHAPE_MESH:
			dump_particle_linked_asset_list(asset->moduleData.initSpawnShapeMesh.m_linkedAssetList, dump);
			dump.dump_array(asset->moduleData.initSpawnShapeMesh.m_meshAssetData, asset->moduleData.initSpawnShapeMesh.m_numMeshAssets);
			break;
		case PARTICLE_MODULE_INIT_SPAWN_SHAPE_SPHERE:
			break;
		case PARTICLE_MODULE_INIT_TAIL:
			break;
		case PARTICLE_MODULE_INIT_VECTOR_FIELD:
			dump_particle_linked_asset_list(asset->moduleData.initVectorField.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_INIT_VOLUMETRIC:
			break;
		case PARTICLE_MODULE_ATTRACTOR:
			break;
		case PARTICLE_MODULE_COLOR_GRAPH:
			for (auto i = 0; i < 8; i++)
			{
				dump.dump_array(asset->moduleData.colorGraph.m_curves[i].controlPoints, asset->moduleData.colorGraph.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_COLOR_LERP:
			break;
		case PARTICLE_MODULE_EMISSION_GRAPH:
			for (auto i = 0; i < 2; i++)
			{
				dump.dump_array(asset->moduleData.emissionGraph.m_curves[i].controlPoints, asset->moduleData.emissionGraph.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_EMISSIVE_GRAPH:
			for (auto i = 0; i < 2; i++)
			{
				dump.dump_array(asset->moduleData.emissiveGraph.m_curves[i].controlPoints, asset->moduleData.emissiveGraph.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_FORCE:
			break;
		case PARTICLE_MODULE_GRAVITY:
			break;
		case PARTICLE_MODULE_INTENSITY_GRAPH:
			for (auto i = 0; i < 2; i++)
			{
				dump.dump_array(asset->moduleData.intensityGraph.m_curves[i].controlPoints, asset->moduleData.intensityGraph.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_PARENT_VELOCITY_GRAPH:
			for (auto i = 0; i < 2; i++)
			{
				dump.dump_array(asset->moduleData.parentVelocityGraph.m_curves[i].controlPoints, asset->moduleData.parentVelocityGraph.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_PHYSICS_LIGHT:
			dump_particle_linked_asset_list(asset->moduleData.physicsLight.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_PHYSICS_RAY_CAST:
			break;
		case PARTICLE_MODULE_POSITION_GRAPH:
			for (auto i = 0; i < 6; i++)
			{
				dump.dump_array(asset->moduleData.positionGraph.m_curves[i].controlPoints, asset->moduleData.positionGraph.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_ROTATION_GRAPH:
			for (auto i = 0; i < 2; i++)
			{
				dump.dump_array(asset->moduleData.rotationGraph.m_curves[i].controlPoints, asset->moduleData.rotationGraph.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_ROTATION_GRAPH_3D:
			for (auto i = 0; i < 6; i++)
			{
				dump.dump_array(asset->moduleData.rotationGraph3D.m_curves[i].controlPoints, asset->moduleData.rotationGraph3D.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_SIZE_GRAPH:
			for (auto i = 0; i < 6; i++)
			{
				dump.dump_array(asset->moduleData.sizeGraph.m_curves[i].controlPoints, asset->moduleData.sizeGraph.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_SIZE_LERP:
			break;
		case PARTICLE_MODULE_VELOCITY_GRAPH:
			for (auto i = 0; i < 6; i++)
			{
				dump.dump_array(asset->moduleData.velocityGraph.m_curves[i].controlPoints, asset->moduleData.velocityGraph.m_curves[i].numControlPoints);
			}
			break;
		case PARTICLE_MODULE_TEST_AGE:
			dump_particle_linked_asset_list(asset->moduleData.testAge.m_eventHandlerData.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_TEST_BIRTH:
			dump_particle_linked_asset_list(asset->moduleData.testBirth.m_eventHandlerData.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_TEST_DEATH:
			dump_particle_linked_asset_list(asset->moduleData.testDeath.m_eventHandlerData.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_TEST_GRAVITY:
			dump_particle_linked_asset_list(asset->moduleData.testGravity.m_eventHandlerData.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_TEST_IMPACT:
			dump_particle_linked_asset_list(asset->moduleData.testImpact.m_eventHandlerData.m_linkedAssetList, dump);
			break;
		case PARTICLE_MODULE_TEST_POS:
			dump_particle_linked_asset_list(asset->moduleData.testPos.m_eventHandlerData.m_linkedAssetList, dump);
			break;
		}
	}

	void dump_module_group(ParticleModuleGroupDef* asset, assetmanager::dumper& dump)
	{
		dump.dump_array(asset->moduleDefs, asset->numModules);
		for (auto i = 0; i < asset->numModules; i++)
		{
			dump_module(&asset->moduleDefs[i], dump);
		}
	}

	void dump_state(ParticleStateDef* asset, assetmanager::dumper& dump)
	{
		dump.dump_array(asset->moduleGroupDefs, 3);
		for (auto i = 0; i < 3; i++)
		{
			dump_module_group(&asset->moduleGroupDefs[i], dump);
		}
	}

	void dump_emitter(ParticleEmitterDef* asset, assetmanager::dumper& dump)
	{
		dump.dump_array(asset->stateDefs, asset->numStates);
		for (auto i = 0; i < asset->numStates; i++)
		{
			dump_state(&asset->stateDefs[i], dump);
		}

		dump.dump_array(asset->fadeCurveDef.controlPoints, asset->fadeCurveDef.numControlPoints);
	}

	void IParticleSystem::dump(ParticleSystemDef* asset)
	{
		assetmanager::dumper dump;

		const auto path = asset->name + ".iw7VFX"s;
		if (!dump.open(path))
		{
			return;
		}

		dump.dump_single(asset);
		dump.dump_string(asset->name);

		dump.dump_array(asset->emitterDefs, asset->numEmitters);
		for (auto i = 0; i < asset->numEmitters; i++)
		{
			dump_emitter(&asset->emitterDefs[i], dump);
		}

		dump.dump_array(asset->scriptedInputNodeDefs, asset->numScriptedInputNodes);

		dump.close();
	}
}