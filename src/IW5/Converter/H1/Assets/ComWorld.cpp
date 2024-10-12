#include "stdafx.hpp"
#include "../Include.hpp"

#include "ComWorld.hpp"

namespace ZoneTool::IW5
{
	namespace H1Converter
	{
		H1::ComWorld* GenerateH1ComWorld(ComWorld* asset, allocator& mem)
		{
			// allocate H1 ComWorld structure
			const auto h1_asset = mem.allocate<H1::ComWorld>();

			h1_asset->name = asset->name;
			h1_asset->isInUse = asset->isInUse;
			h1_asset->primaryLightCount = asset->primaryLightCount;
			h1_asset->primaryLights = mem.allocate<H1::ComPrimaryLight>(h1_asset->primaryLightCount);
			for (unsigned int i = 0; i < h1_asset->primaryLightCount; i++)
			{
				h1_asset->primaryLights[i].type = static_cast<H1::GfxLightType>(asset->primaryLights[i].type); // don't think there is a need for converting
				h1_asset->primaryLights[i].canUseShadowMap = asset->primaryLights[i].canUseShadowMap;
				h1_asset->primaryLights[i].exponent = asset->primaryLights[i].exponent;
				// unk1
				memcpy(&h1_asset->primaryLights[i].color, &asset->primaryLights[i].color, sizeof(float[3]));
				memcpy(&h1_asset->primaryLights[i].dir, &asset->primaryLights[i].dir, sizeof(float[3]));
				memcpy(&h1_asset->primaryLights[i].up, &asset->primaryLights[i].up, sizeof(float[3]));
				memcpy(&h1_asset->primaryLights[i].origin, &asset->primaryLights[i].origin, sizeof(float[3]));
				// pad
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
				h1_asset->primaryLights[i].defName = asset->primaryLights[i].defName && *asset->primaryLights[i].defName ? asset->primaryLights[i].defName : nullptr;

				if (h1_asset->primaryLights[i].type == H1::GFX_LIGHT_TYPE_SPOT)
				{
					h1_asset->primaryLights[i].canUseShadowMap = 1;
					h1_asset->primaryLights[i].physicallyBased = 1;
					h1_asset->primaryLights[i].lightingState = 0;

					h1_asset->primaryLights[i].cucScaleVector[0] = 1.0f;
					h1_asset->primaryLights[i].cucScaleVector[1] = 1.0f;

					h1_asset->primaryLights[i].bulbRadius = 3.0f;
					h1_asset->primaryLights[i].bulbLength[0] = 0.0f;
					h1_asset->primaryLights[i].bulbLength[1] = 0.0f;
					h1_asset->primaryLights[i].bulbLength[2] = 0.0f;
					h1_asset->primaryLights[i].fadeOffset[0] = 0.0f;
					h1_asset->primaryLights[i].fadeOffset[1] = 0.0f;

					// idk why...
					for (auto c = 0; c < 3; c++)
					{
						h1_asset->primaryLights[i].color[c] *= 10000.0f;
					}

					if (!h1_asset->primaryLights[i].defName)
					{
						h1_asset->primaryLights[i].defName = mem.duplicate_string("light_point_linear");
					}
				}
				else if (h1_asset->primaryLights[i].type == H1::GFX_LIGHT_TYPE_OMNI)
				{
					h1_asset->primaryLights[i].canUseShadowMap = 1;
					h1_asset->primaryLights[i].physicallyBased = 1;
					h1_asset->primaryLights[i].lightingState = 0;

					h1_asset->primaryLights[i].cucScaleVector[0] = 1.0f;
					h1_asset->primaryLights[i].cucScaleVector[1] = 1.0f;

					h1_asset->primaryLights[i].bulbRadius = 3.0f;
					h1_asset->primaryLights[i].bulbLength[0] = 0.0f;
					h1_asset->primaryLights[i].bulbLength[1] = 0.0f;
					h1_asset->primaryLights[i].bulbLength[2] = 0.0f;
					h1_asset->primaryLights[i].fadeOffset[0] = 0.0f;
					h1_asset->primaryLights[i].fadeOffset[1] = 0.0f;

					// idk why...
					for (auto c = 0; c < 3; c++)
					{
						h1_asset->primaryLights[i].color[c] *= 10000.0f;
					}

					if (!h1_asset->primaryLights[i].defName)
					{
						h1_asset->primaryLights[i].defName = mem.duplicate_string("light_point_linear");
					}
				}
			}

			h1_asset->primaryLightEnvCount = h1_asset->primaryLightCount + 1;
			h1_asset->primaryLightEnvs = mem.allocate<H1::ComPrimaryLightEnv>(h1_asset->primaryLightEnvCount);
			
			for (unsigned int i = 1; i < h1_asset->primaryLightCount; i++)
			{
				h1_asset->primaryLightEnvs[i].numIndices = 1;

				//if (h1_asset->primaryLights[i].type == H1::GFX_LIGHT_TYPE_SPOT || h1_asset->primaryLights[i].type == H1::GFX_LIGHT_TYPE_OMNI)
				//{
				//	h1_asset->primaryLightEnvs[i].primaryLightIndices[0] = 1; // always use first primary light index, since this no no work
				//}
				//else
				{
					h1_asset->primaryLightEnvs[i].primaryLightIndices[0] = i;
				}
			}

			h1_asset->primaryLightEnvs[h1_asset->primaryLightEnvCount - 1].numIndices = 1;
			h1_asset->primaryLightEnvs[h1_asset->primaryLightEnvCount - 1].primaryLightIndices[0] = 2047;

			return h1_asset;
		}

		H1::ComWorld* convert(ComWorld* asset, allocator& allocator)
		{
			// generate h1 comworld
			return GenerateH1ComWorld(asset, allocator);
		}
	}
}