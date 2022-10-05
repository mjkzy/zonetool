#include "stdafx.hpp"
#include "H1/Assets/ComWorld.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		H1::ComWorld* GenerateH1ComWorld(ComWorld* asset, ZoneMemory* mem)
		{
			// allocate H1 ComWorld structure
			const auto h1_asset = mem->Alloc<H1::ComWorld>();

			h1_asset->name = asset->name;
			h1_asset->isInUse = asset->isInUse;
			h1_asset->primaryLightCount = asset->primaryLightCount;
			h1_asset->primaryLights = mem->Alloc<H1::ComPrimaryLight>(h1_asset->primaryLightCount);
			for (unsigned int i = 0; i < h1_asset->primaryLightCount; i++)
			{
				h1_asset->primaryLights[i].type = asset->primaryLights[i].type;
				h1_asset->primaryLights[i].canUseShadowMap = asset->primaryLights[i].canUseShadowMap;
				h1_asset->primaryLights[i].exponent = asset->primaryLights[i].exponent;
				h1_asset->primaryLights[i].unused = asset->primaryLights[i].unused;
				memcpy(&h1_asset->primaryLights[i].color, &asset->primaryLights[i].color, sizeof(float[3]));
				memcpy(&h1_asset->primaryLights[i].dir, &asset->primaryLights[i].dir, sizeof(float[3]));
				memcpy(&h1_asset->primaryLights[i].up, &asset->primaryLights[i].up, sizeof(float[3]));
				memcpy(&h1_asset->primaryLights[i].origin, &asset->primaryLights[i].origin, sizeof(float[3]));
				h1_asset->primaryLights[i].radius = asset->primaryLights[i].radius;
				h1_asset->primaryLights[i].cosHalfFovOuter = asset->primaryLights[i].cosHalfFovOuter;
				h1_asset->primaryLights[i].cosHalfFovInner = asset->primaryLights[i].cosHalfFovInner;
				h1_asset->primaryLights[i].cosHalfFovExpanded = asset->primaryLights[i].cosHalfFovExpanded;
				h1_asset->primaryLights[i].rotationLimit = asset->primaryLights[i].rotationLimit;
				h1_asset->primaryLights[i].translationLimit = asset->primaryLights[i].translationLimit;
				h1_asset->primaryLights[i].cucRotationOffsetRad = 0;
				h1_asset->primaryLights[i].cucRotationSpeedRad = 0;
				memset(&h1_asset->primaryLights[i].cucScrollVector, 0, sizeof(float[2]));
				memset(&h1_asset->primaryLights[i].cucScaleVector, 0, sizeof(float[2]));
				memset(&h1_asset->primaryLights[i].cucTransVector, 0, sizeof(float[2]));
				h1_asset->primaryLights[i].defName = asset->primaryLights[i].defName;

				if (h1_asset->primaryLights[i].type == 2)
				{
					h1_asset->primaryLights[i].cucScaleVector[0] = 1.0f;
					h1_asset->primaryLights[i].cucScaleVector[1] = 1.0f;
				}
			}
			h1_asset->primaryLightEnvCount = h1_asset->primaryLightCount;
			h1_asset->primaryLightEnvs = mem->Alloc<H1::ComPrimaryLightEnv>(h1_asset->primaryLightEnvCount);
			for (unsigned int i = 1; i < h1_asset->primaryLightEnvCount; i++)
			{
				h1_asset->primaryLightEnvs[i].numIndices = 4;
				h1_asset->primaryLightEnvs[i].primaryLightIndices[0] = i;
				h1_asset->primaryLightEnvs[i].primaryLightIndices[1] = i;
				h1_asset->primaryLightEnvs[i].primaryLightIndices[2] = i;
				h1_asset->primaryLightEnvs[i].primaryLightIndices[3] = i;
			}

			return h1_asset;
		}

		void IComWorld::dump(ComWorld* asset, ZoneMemory* mem)
		{
			// generate h1 comworld
			auto h1_asset = GenerateH1ComWorld(asset, mem);

			// dump h1 comworld
			H1::IComWorld::dump(h1_asset);
		}
	}
}