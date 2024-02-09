#include "stdafx.hpp"
#include "IW4/Assets/GfxWorld.hpp"

namespace ZoneTool
{
	namespace IW3
	{
		IW4::GfxWorld* GenerateIW4GfxWorld(GfxWorld* world, allocator& mem)
		{
			auto map = mem.allocate<IW4::GfxWorld>();
			auto sky = mem.allocate<IW4::GfxSky>();
			ZeroMemory(sky, sizeof(IW4::GfxSky));
			ZeroMemory(map, sizeof(IW4::GfxWorld));

			map->name = world->name;
			map->baseName = world->baseName;
			map->planeCount = world->planeCount;
			map->nodeCount = world->nodeCount;
			map->surfaceCount = world->surfaceCount;

			map->skyCount = 1;
			map->skies = sky;

			sky->skyImage = (IW4::GfxImage*)world->skyImage;
			sky->skySamplerState = world->skySamplerState & 0xFF;
			sky->skyStartSurfs = (uint32_t*)world->skyStartSurfs;
			sky->skySurfCount = world->skySurfCount;

			map->lastSunPrimaryLightIndex = world->sunPrimaryLightIndex;
			map->primaryLightCount = world->primaryLightCount;

			// map->dpvsPlanes = world->dpvsPlanes;
			memcpy(&map->dpvsPlanes, &world->dpvsPlanes, sizeof world->dpvsPlanes);

			// AABBTree data is stored as part of the cells.
			// However, in IW4 it's not, so we have to extract the data
			if (world->cells)
			{
				map->aabbTreeCounts = mem.allocate<IW4::GfxCellTreeCount>(world->dpvsPlanes.cellCount);
				map->aabbTree = mem.allocate<IW4::GfxCellTree>(world->dpvsPlanes.cellCount);
				map->cells = mem.allocate<IW4::GfxCell>(world->dpvsPlanes.cellCount);

				for (int i = 0; i < world->dpvsPlanes.cellCount; ++i)
				{
					map->aabbTreeCounts[i].aabbTreeCount = world->cells[i].aabbTreeCount;

					bounds::compute(world->cells[i].mins, world->cells[i].maxs, &map->cells[i].bounds.midPoint);
					map->cells[i].portalCount = world->cells[i].portalCount;
					map->cells[i].reflectionProbeCount = world->cells[i].reflectionProbeCount;
					map->cells[i].reflectionProbes = world->cells[i].reflectionProbes;

					if (world->cells[i].aabbTree)
					{
						map->aabbTree[i].aabbtree = mem.allocate<IW4::GfxAabbTree>(world->cells[i].aabbTreeCount);
						std::memcpy(map->aabbTree[i].aabbtree, world->cells[i].aabbTree, sizeof(IW4::GfxAabbTree) * world->cells[i].aabbTreeCount);

						for (int j = 0; j < world->cells[i].aabbTreeCount; ++j)
						{
							static_assert(sizeof IW4::GfxAabbTree == sizeof IW3::GfxAabbTree, "Size mismatch");
							bounds::compute(world->cells[i].aabbTree[j].mins, world->cells[i].aabbTree[j].maxs, &map->aabbTree[i].aabbtree[j].bounds.midPoint);
						}
					}

					if (world->cells[i].portals)
					{
						map->cells[i].portals = mem.allocate<IW4::GfxPortal>(world->cells[i].portalCount);

						// Map all portals, so we have them ready for the next loop (might be unnecessary, as they are mapped at runtime)
						std::unordered_map<std::uintptr_t, std::uintptr_t> portalMap = { { 0, 0 } };
						for (int j = 0; j < world->cells[i].portalCount; ++j)
						{
							portalMap[(std::uintptr_t)&world->cells[i].portals[j]] = (std::uintptr_t)&map->cells[i].portals[j];
						}

						for (int j = 0; j < world->cells[i].portalCount; ++j)
						{
							IW3::GfxPortal* portal = &world->cells[i].portals[j];
							IW4::GfxPortal* destPortal = &map->cells[i].portals[j];

							destPortal->cellIndex = static_cast<unsigned short>(portal->cell - world->cells);
							if (destPortal->cellIndex >= static_cast<unsigned short>(world->dpvsPlanes.cellCount))
							{
								ZONETOOL_FATAL("Unable to calculate cell index. This should not happen!\n");
								destPortal->cellIndex = 0;
							}

							destPortal->vertices = portal->vertices;
							destPortal->vertexCount = portal->vertexCount;

							destPortal->writable.isQueued = portal->writable.isQueued;
							destPortal->writable.isAncestor = portal->writable.isAncestor;
							destPortal->writable.recursionDepth = portal->writable.recursionDepth;
							destPortal->writable.hullPointCount = portal->writable.hullPointCount;
							destPortal->writable.hullPoints = portal->writable.hullPoints;

							if (portalMap.find((std::uintptr_t)portal->writable.queuedParent) != portalMap.end())
							{
								destPortal->writable.queuedParent = (IW4::GfxPortal*)portalMap[(std::uintptr_t)portal->writable.queuedParent];
							}
							else
							{
								if (portal->writable.queuedParent) ZONETOOL_ERROR("Unmapped portal. This shouldn't happen. Nulling it...\n");
								destPortal->writable.queuedParent = nullptr;
							}

							std::memcpy(destPortal->plane.coeffs, portal->plane.coeffs, sizeof(destPortal->plane.coeffs));
							std::memcpy(destPortal->hullAxis, portal->hullAxis, sizeof(destPortal->hullAxis));
						}
					}
				}
			}

			map->worldDraw.reflectionProbeCount = world->reflectionProbeCount;
			map->worldDraw.reflectionProbeTextures = (IW4::GfxTexture*)world->reflectionProbeTextures;
			map->worldDraw.lightmapCount = world->lightmapCount;
			map->worldDraw.lightmaps = mem.allocate<IW4::GfxLightmapArray>(world->lightmapCount); // (IW4::GfxLightmapArray*)world->lightmaps;

			for (auto i = 0; i < world->lightmapCount; i++)
			{
				if (world->lightmaps[i].primary)
				{
					IGfxImage::dump(world->lightmaps[i].primary);
					map->worldDraw.lightmaps[i].primary = IGfxImage::GenerateIW4Image(world->lightmaps[i].primary, mem);
				}

				if (world->lightmaps[i].secondary)
				{
					IGfxImage::dump(world->lightmaps[i].secondary);
					map->worldDraw.lightmaps[i].secondary = IGfxImage::GenerateIW4Image(world->lightmaps[i].secondary, mem);
				}
			}

			map->worldDraw.lightmapPrimaryTextures = (IW4::GfxTexture*)world->lightmapPrimaryTextures;
			map->worldDraw.lightmapSecondaryTextures = (IW4::GfxTexture*)world->lightmapSecondaryTextures;
			map->worldDraw.skyImage = IGfxImage::GenerateIW4Image(world->skyImage, mem);
			map->worldDraw.outdoorImage = IGfxImage::GenerateIW4Image(world->outdoorImage, mem);
			map->worldDraw.vertexCount = world->vertexCount;
			memcpy(&map->worldDraw.vd, &world->vd, sizeof world->vd);
			map->worldDraw.vertexLayerDataSize = world->vertexLayerDataSize;
			memcpy(&map->worldDraw.vld, &world->vld, sizeof world->vld);
			map->worldDraw.indexCount = world->indexCount;
			map->worldDraw.indices = world->indices;

			// Split reflection images and probes
			if (world->reflectionProbes)
			{
				map->worldDraw.reflectionImages = mem.allocate<IW4::GfxImage*>(world->reflectionProbeCount);
				map->worldDraw.reflectionProbes = mem.allocate<IW4::GfxReflectionProbe>(world->reflectionProbeCount);

				for (unsigned int i = 0; i < world->reflectionProbeCount; ++i)
				{
					map->worldDraw.reflectionImages[i] = IGfxImage::GenerateIW4Image(world->reflectionProbes[i].reflectionImage, mem);
					IGfxImage::dump(world->reflectionProbes[i].reflectionImage);

					std::memcpy(map->worldDraw.reflectionProbes[i].offset, world->reflectionProbes[i].origin, sizeof(map->worldDraw.reflectionProbes[i].offset));
				}
			}

			memcpy(&map->lightGrid, &world->lightGrid, sizeof world->lightGrid);

			assert(sizeof IW3::GfxBrushModel == 56);
			assert(sizeof IW4::GfxBrushModel == 60);

			map->modelCount = world->modelCount;
			if (world->models)
			{
				map->models = mem.allocate<IW4::GfxBrushModel>(world->modelCount);

				for (int i = 0; i < world->modelCount; ++i)
				{
					bounds::compute(world->models[i].bounds[0], world->models[i].bounds[1], &map->models[i].bounds.midPoint);

					float* halfSize = map->models[i].bounds.halfSize;
					map->models[i].radius = static_cast<float>(std::sqrt(std::pow(halfSize[0], 2) + std::pow(halfSize[1], 2) + std::pow(halfSize[2], 2)));

					map->models[i].surfaceCount = world->models[i].surfaceCount;
					map->models[i].startSurfIndex = world->models[i].startSurfIndex;
					map->models[i].surfaceCountNoDecal = world->models[i].surfaceCountNoDecal;
				}
			}

			bounds::compute(world->mins, world->maxs, &map->bounds.midPoint);

			map->checksum = world->checksum;
			map->materialMemoryCount = world->materialMemoryCount;
			map->materialMemory = (IW4::MaterialMemory*)world->materialMemory;
			memcpy(&map->sun, &world->sun, sizeof world->sun);

			std::memcpy(map->outdoorLookupMatrix, world->outdoorLookupMatrix, sizeof(map->outdoorLookupMatrix));
			map->outdoorImage = map->worldDraw.outdoorImage;

			IGfxImage::dump(world->outdoorImage);

			map->cellCasterBits[0] = world->cellCasterBits;
			map->cellCasterBits[1] = world->cellCasterBits; // This mustn't be null!

			map->sceneDynModel = (IW4::GfxSceneDynModel*)world->sceneDynModel;
			map->sceneDynBrush = (IW4::GfxSceneDynBrush*)world->sceneDynBrush;

			map->primaryLightEntityShadowVis = world->primaryLightEntityShadowVis;
			map->primaryLightDynEntShadowVis[0] = world->primaryLightDynEntShadowVis[0];
			map->primaryLightDynEntShadowVis[1] = world->primaryLightDynEntShadowVis[1];
			map->primaryLightForModelDynEnt = (unsigned char*)world->nonSunPrimaryLightForModelDynEnt;

			map->shadowGeom = (IW4::GfxShadowGeometry*)world->shadowGeom;
			map->lightRegion = (IW4::GfxLightRegion*)world->lightRegion;

			map->dpvs.smodelCount = world->dpvs.smodelCount;
			map->dpvs.staticSurfaceCount = world->dpvs.staticSurfaceCount;
			map->dpvs.staticSurfaceCountNoDecal = world->dpvs.staticSurfaceCountNoDecal;

			// Not sure if correct
			// update: slightly more sure but not much lol
			map->dpvs.litOpaqueSurfsBegin = world->dpvs.litSurfsBegin;
			map->dpvs.litOpaqueSurfsEnd = world->dpvs.decalSurfsEnd;

			// these don't exist in iw3 so skip
			map->dpvs.litTransSurfsBegin = world->dpvs.decalSurfsEnd;
			map->dpvs.litTransSurfsEnd = world->dpvs.decalSurfsEnd;

			// Skip those as well
			map->dpvs.shadowCasterSurfsBegin = world->dpvs.decalSurfsEnd;
			map->dpvs.shadowCasterSurfsEnd = world->dpvs.decalSurfsEnd;

			map->dpvs.emissiveSurfsBegin = world->dpvs.emissiveSurfsBegin;
			map->dpvs.emissiveSurfsEnd = world->dpvs.emissiveSurfsEnd;
			map->dpvs.smodelVisDataCount = world->dpvs.smodelVisDataCount;
			map->dpvs.surfaceVisDataCount = world->dpvs.surfaceVisDataCount;

			std::memcpy(map->dpvs.smodelVisData, world->dpvs.smodelVisData, sizeof(map->dpvs.smodelVisData));
			std::memcpy(map->dpvs.surfaceVisData, world->dpvs.surfaceVisData, sizeof(map->dpvs.surfaceVisData));

			map->dpvs.sortedSurfIndex = world->dpvs.sortedSurfIndex;

			if (world->dpvs.smodelInsts)
			{
				map->dpvs.smodelInsts = mem.allocate<IW4::GfxStaticModelInst>(world->dpvs.smodelCount);

				for (unsigned int i = 0; i < world->dpvs.smodelCount; ++i)
				{
					bounds::compute(world->dpvs.smodelInsts[i].mins, world->dpvs.smodelInsts[i].maxs, &map->dpvs.smodelInsts[i].bounds.midPoint);

					map->dpvs.smodelInsts[i].lightingOrigin[0] = map->dpvs.smodelInsts[i].bounds.midPoint[0];
					map->dpvs.smodelInsts[i].lightingOrigin[1] = map->dpvs.smodelInsts[i].bounds.midPoint[1];
					map->dpvs.smodelInsts[i].lightingOrigin[2] = map->dpvs.smodelInsts[i].bounds.midPoint[2];
				}
			}

			if (world->dpvs.surfaces)
			{
				map->dpvs.surfaces = mem.allocate<IW4::GfxSurface>(world->surfaceCount);
				map->dpvs.surfacesBounds = mem.allocate<IW4::GfxSurfaceBounds>(world->surfaceCount);

				assert(sizeof(IW3::srfTriangles_t) == sizeof(IW4::srfTriangles_t));

				for (auto i = 0; i < world->surfaceCount; ++i)
				{
					std::memcpy(&map->dpvs.surfaces[i].tris, &world->dpvs.surfaces[i].tris, sizeof(IW4::srfTriangles_t));
					map->dpvs.surfaces[i].material = (IW4::Material*)world->dpvs.surfaces[i].material;
					map->dpvs.surfaces[i].lightmapIndex = world->dpvs.surfaces[i].lightmapIndex;
					map->dpvs.surfaces[i].reflectionProbeIndex = world->dpvs.surfaces[i].reflectionProbeIndex;
					map->dpvs.surfaces[i].primaryLightIndex = world->dpvs.surfaces[i].primaryLightIndex;
					map->dpvs.surfaces[i].flags = world->dpvs.surfaces[i].flags;

					bounds::compute(world->dpvs.surfaces[i].bounds[0], world->dpvs.surfaces[i].bounds[1], &map->dpvs.surfacesBounds[i].bounds.midPoint);

					assert(map->dpvs.surfaces[i].material != nullptr);
				}
			}

			if (world->dpvs.smodelDrawInsts)
			{
				map->dpvs.smodelDrawInsts = mem.allocate<IW4::GfxStaticModelDrawInst>(world->dpvs.smodelCount);

				for (unsigned int i = 0; i < world->dpvs.smodelCount; ++i)
				{
					std::memcpy(&map->dpvs.smodelDrawInsts[i].placement, &world->dpvs.smodelDrawInsts[i].placement, sizeof(GfxPackedPlacement));
					std::memcpy(map->dpvs.smodelDrawInsts[i].cacheId, world->dpvs.smodelDrawInsts[i].smodelCacheIndex, sizeof(map->dpvs.smodelDrawInsts[i].cacheId));

					map->dpvs.smodelDrawInsts[i].model = (IW4::XModel*)world->dpvs.smodelDrawInsts[i].model;
					map->dpvs.smodelDrawInsts[i].cullDist = static_cast<unsigned short>(world->dpvs.smodelDrawInsts[i].cullDist);
					map->dpvs.smodelDrawInsts[i].reflectionProbeIndex = world->dpvs.smodelDrawInsts[i].reflectionProbeIndex;
					map->dpvs.smodelDrawInsts[i].primaryLightIndex = world->dpvs.smodelDrawInsts[i].primaryLightIndex;
					map->dpvs.smodelDrawInsts[i].lightingHandle = world->dpvs.smodelDrawInsts[i].lightingHandle;
					map->dpvs.smodelDrawInsts[i].flags = world->dpvs.smodelDrawInsts[i].flags;

					// This has been moved
					if (world->dpvs.smodelInsts) map->dpvs.smodelDrawInsts[i].groundLighting.packed = world->dpvs.smodelInsts[i].groundLighting.packed;
				}
			}

			map->dpvs.surfaceMaterials = (IW4::GfxDrawSurf*)world->dpvs.surfaceMaterials;
			map->dpvs.surfaceCastsSunShadow = world->dpvs.surfaceCastsSunShadow;
			map->dpvs.usageCount = world->dpvs.usageCount;

			memcpy(&map->dpvsDyn, &world->dpvsDyn, sizeof world->dpvsDyn);

			// Should we set that to true? :O
			map->fogTypesAllowed = 0x1; // iw4_credits has 0x3

			map->sortKeyLitDecal = 0x6;
			map->sortKeyEffectDecal = 0x27;
			map->sortKeyEffectAuto = 0x30;
			map->sortKeyDistortion = 0x2b;

			// sort triangles & vertices
			auto tri_index = 0;
			map->worldDraw.indices = mem.allocate<unsigned short>(map->worldDraw.indexCount);

			for (auto i = 0; i < map->surfaceCount; i++)
			{
				auto* surface = &map->dpvs.surfaces[i];

				// triangles
				std::memcpy(&map->worldDraw.indices[tri_index], &world->indices[surface->tris.baseIndex], surface->tris.triCount * 6);
				surface->tris.baseIndex = tri_index;
				tri_index += surface->tris.triCount * 3;
			}

			if (tri_index != map->worldDraw.indexCount)
			{
				ZONETOOL_WARNING("Warning: Didn't sort all indicies for draw");
			}

			// Specify that it's a custom map
			map->checksum = 0xDEADBEEF;

			return map;
		}

		void IGfxWorld::dump(GfxWorld* asset)
		{
			// generate iw4 gfxworld
			allocator allocator;
			auto iw4_asset = GenerateIW4GfxWorld(asset, allocator);

			// dump gfxworld
			IW4::IGfxWorld::dump(iw4_asset);
		}
	}
}