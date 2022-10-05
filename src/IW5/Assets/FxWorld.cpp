#include "stdafx.hpp"
#include "H1/Assets/FxWorld.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		H1::FxWorld* GenerateH1FxWorld(FxWorld* asset, ZoneMemory* mem)
		{
			// allocate H1 FxWorld structure
			const auto h1_asset = mem->Alloc<H1::FxWorld>();

			h1_asset->name = asset->name;

			h1_asset->glassSys.time = asset->glassSys.time;
			h1_asset->glassSys.prevTime = asset->glassSys.prevTime;
			h1_asset->glassSys.defCount = asset->glassSys.defCount;
			h1_asset->glassSys.pieceLimit = asset->glassSys.pieceLimit;
			h1_asset->glassSys.pieceWordCount = asset->glassSys.pieceWordCount;
			h1_asset->glassSys.initPieceCount = asset->glassSys.initPieceCount;
			h1_asset->glassSys.cellCount = asset->glassSys.cellCount;
			h1_asset->glassSys.activePieceCount = asset->glassSys.activePieceCount;
			h1_asset->glassSys.firstFreePiece = asset->glassSys.firstFreePiece;
			h1_asset->glassSys.geoDataLimit = asset->glassSys.geoDataLimit;
			h1_asset->glassSys.geoDataCount = asset->glassSys.geoDataCount;
			h1_asset->glassSys.initGeoDataCount = asset->glassSys.initGeoDataCount;

			h1_asset->glassSys.defs = mem->Alloc<H1::FxGlassDef>(h1_asset->glassSys.defCount);
			for (unsigned int i = 0; i < h1_asset->glassSys.defCount; i++)
			{
				h1_asset->glassSys.defs[i].halfThickness = asset->glassSys.defs[i].halfThickness;
				memcpy(&h1_asset->glassSys.defs[i].texVecs, &asset->glassSys.defs[i].texVecs, sizeof(float[2][2]));
				h1_asset->glassSys.defs[i].color.packed = asset->glassSys.defs[i].color.packed;
				h1_asset->glassSys.defs[i].material = reinterpret_cast<H1::Material*>(asset->glassSys.defs[i].material);
				h1_asset->glassSys.defs[i].materialShattered = reinterpret_cast<H1::Material*>(asset->glassSys.defs[i].materialShattered);
				h1_asset->glassSys.defs[i].physPreset = reinterpret_cast<H1::PhysPreset*>(asset->glassSys.defs[i].physPreset);

				h1_asset->glassSys.defs[i].pieceBreakEffect = nullptr;
				h1_asset->glassSys.defs[i].shatterEffect = nullptr;
				h1_asset->glassSys.defs[i].shatterSmallEffect = nullptr;
				h1_asset->glassSys.defs[i].crackDecalEffect = nullptr;
				h1_asset->glassSys.defs[i].damagedSound = nullptr;
				h1_asset->glassSys.defs[i].destroyedSound = nullptr;
				h1_asset->glassSys.defs[i].destroyedQuietSound = nullptr;
				h1_asset->glassSys.defs[i].numCrackRings = 0;
				h1_asset->glassSys.defs[i].isOpaque = 0;
			}

			h1_asset->glassSys.piecePlaces = mem->Alloc<H1::FxGlassPiecePlace>(h1_asset->glassSys.pieceLimit);
			for (unsigned int i = 0; i < h1_asset->glassSys.pieceLimit; i++)
			{
				memcpy(&h1_asset->glassSys.piecePlaces[i], &asset->glassSys.piecePlaces[i], sizeof(IW5::FxGlassPiecePlace));
			}

			h1_asset->glassSys.pieceStates = mem->Alloc<H1::FxGlassPieceState>(h1_asset->glassSys.pieceLimit);
			for (unsigned int i = 0; i < h1_asset->glassSys.pieceLimit; i++)
			{
				memcpy(&h1_asset->glassSys.pieceStates[i], &asset->glassSys.pieceStates[i], sizeof(IW5::FxGlassPieceState));
			}

			h1_asset->glassSys.pieceDynamics = mem->Alloc<H1::FxGlassPieceDynamics>(h1_asset->glassSys.pieceLimit);
			for (unsigned int i = 0; i < h1_asset->glassSys.pieceLimit; i++)
			{
				h1_asset->glassSys.pieceDynamics[i].fallTime = asset->glassSys.pieceDynamics[i].fallTime;
				h1_asset->glassSys.pieceDynamics[i].physObjId = asset->glassSys.pieceDynamics[i].physObjId;
				h1_asset->glassSys.pieceDynamics[i].physJointId = asset->glassSys.pieceDynamics[i].physJointId;
				memcpy(&h1_asset->glassSys.pieceDynamics[i].vel, &asset->glassSys.pieceDynamics[i].vel, sizeof(float[3]));
				memcpy(&h1_asset->glassSys.pieceDynamics[i].avel, &asset->glassSys.pieceDynamics[i].avel, sizeof(float[3]));
			}

			h1_asset->glassSys.geoData = mem->Alloc<H1::FxGlassGeometryData>(h1_asset->glassSys.geoDataLimit);
			for (unsigned int i = 0; i < h1_asset->glassSys.geoDataLimit; i++)
			{
				memcpy(&h1_asset->glassSys.geoData[i], &asset->glassSys.geoData[i], sizeof(IW5::FxGlassGeometryData));
			}

			h1_asset->glassSys.isInUse = mem->Alloc<unsigned int>(h1_asset->glassSys.pieceWordCount);
			for (unsigned int i = 0; i < h1_asset->glassSys.pieceWordCount; i++)
			{
				h1_asset->glassSys.isInUse[i] = asset->glassSys.isInUse[i];
			}

			h1_asset->glassSys.cellBits = mem->Alloc<unsigned int>(h1_asset->glassSys.pieceWordCount * h1_asset->glassSys.cellCount);
			for (unsigned int i = 0; i < h1_asset->glassSys.pieceWordCount * h1_asset->glassSys.cellCount; i++)
			{
				h1_asset->glassSys.cellBits[i] = asset->glassSys.cellBits[i];
			}

			h1_asset->glassSys.visData = mem->Alloc<unsigned char>((h1_asset->glassSys.pieceLimit + 15) & 0xFFFFFFF0);
			for (unsigned int i = 0; i < ((h1_asset->glassSys.pieceLimit + 15) & 0xFFFFFFF0); i++)
			{
				h1_asset->glassSys.visData[i] = asset->glassSys.visData[i];
			}

			h1_asset->glassSys.linkOrg = reinterpret_cast<float(*)[3]>(asset->glassSys.linkOrg);

			h1_asset->glassSys.halfThickness = mem->Alloc<float>((h1_asset->glassSys.pieceLimit + 3) & 0xFFFFFFFC);
			for (unsigned int i = 0; i < ((h1_asset->glassSys.pieceLimit + 3) & 0xFFFFFFFC); i++)
			{
				h1_asset->glassSys.halfThickness[i] = asset->glassSys.halfThickness[i];
			}

			h1_asset->glassSys.lightingHandles = mem->Alloc<unsigned short>(h1_asset->glassSys.initPieceCount);
			for (unsigned int i = 0; i < h1_asset->glassSys.initPieceCount; i++)
			{
				h1_asset->glassSys.lightingHandles[i] = asset->glassSys.lightingHandles[i];
			}

			h1_asset->glassSys.initPieceStates = mem->Alloc<H1::FxGlassInitPieceState>(h1_asset->glassSys.initPieceCount);
			for (unsigned int i = 0; i < h1_asset->glassSys.initPieceCount; i++)
			{
				// iw5 struct maybe wrong
				memcpy(&h1_asset->glassSys.initPieceStates[i].frame, &asset->glassSys.initPieceStates[i].frame, sizeof(IW5::FxSpatialFrame));
				h1_asset->glassSys.initPieceStates[i].radius = asset->glassSys.initPieceStates[i].radius;
				h1_asset->glassSys.initPieceStates[i].texCoordOrigin[0] = asset->glassSys.initPieceStates[i].texCoordOrigin[0];
				h1_asset->glassSys.initPieceStates[i].texCoordOrigin[1] = asset->glassSys.initPieceStates[i].texCoordOrigin[1];
				h1_asset->glassSys.initPieceStates[i].supportMask = asset->glassSys.initPieceStates[i].supportMask;
				h1_asset->glassSys.initPieceStates[i].areaX2 = asset->glassSys.initPieceStates[i].areaX2;
				h1_asset->glassSys.initPieceStates[i].lightingIndex = asset->glassSys.initPieceStates[i].lightingIndex;
				h1_asset->glassSys.initPieceStates[i].defIndex = asset->glassSys.initPieceStates[i].defIndex;
				h1_asset->glassSys.initPieceStates[i].vertCount = asset->glassSys.initPieceStates[i].vertCount;
				h1_asset->glassSys.initPieceStates[i].fanDataCount = 0;
			}

			h1_asset->glassSys.initGeoData = mem->Alloc<H1::FxGlassGeometryData>(h1_asset->glassSys.initGeoDataCount);
			for (unsigned int i = 0; i < h1_asset->glassSys.initGeoDataCount; i++)
			{
				memcpy(&h1_asset->glassSys.initGeoData[i], &asset->glassSys.initGeoData[i], sizeof(IW5::FxGlassGeometryData));
			}

			h1_asset->glassSys.needToCompactData = asset->glassSys.needToCompactData;
			h1_asset->glassSys.initCount = asset->glassSys.initCount;
			h1_asset->glassSys.effectChanceAccum = asset->glassSys.effectChanceAccum;
			h1_asset->glassSys.lastPieceDeletionTime = asset->glassSys.lastPieceDeletionTime;

			return h1_asset;
		}

		void IFxWorld::dump(FxWorld* asset, ZoneMemory* mem)
		{
			// generate h1 fxworld
			auto h1_asset = GenerateH1FxWorld(asset, mem);

			// dump h1 fxworld
			H1::IFxWorld::dump(h1_asset);
		}
	}
}