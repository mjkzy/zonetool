#include "stdafx.hpp"
#include "../Include.hpp"

#include "PhysCollmap.hpp"

#include "H1/Utils/PhysWorld/earcut.hpp"
#include <unordered_set>

namespace
{
	namespace utils::cryptography::jenkins_one_at_a_time
	{
		unsigned int compute(const char* key, const size_t len)
		{
			unsigned int hash, i;
			for (hash = i = 0; i < len; ++i)
			{
				hash += key[i];
				hash += (hash << 10);
				hash ^= (hash >> 6);
			}
			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);
			return hash;
		}

		unsigned int compute(const std::string& data)
		{
			return compute(data.data(), data.size());
		}
	}
}


namespace ZoneTool::IW5
{
	namespace H1Converter
	{
		namespace
		{
			struct triangle_t
			{
				int index;
				int verts[3];
			};

			struct polygon_t
			{
				float(*pts)[3];
				unsigned int ptCount;
			};

			struct axial_planes_t
			{
				float planes[6][4];
			};

			int hash_triangle(const triangle_t& tri, const std::vector<ZoneTool::H1::dmFloat4>& phys_verticies)
			{
				float verts[3][3]{};
				std::memcpy(&verts[0], &phys_verticies[tri.verts[0]], sizeof(float[3]));
				std::memcpy(&verts[1], &phys_verticies[tri.verts[1]], sizeof(float[3]));
				std::memcpy(&verts[2], &phys_verticies[tri.verts[2]], sizeof(float[3]));

				std::string data;
				data.append(reinterpret_cast<char*>(&verts[0]), sizeof(float[3][3]));

				return utils::cryptography::jenkins_one_at_a_time::compute(data);
			}

			void get_winding_for_brush_face(cplane_s* planes, const Bounds* bounds,
				const cbrush_t* brush, unsigned int brush_side,
				polygon_t* out_winding, int max_verts, const axial_planes_t* axial_planes)
			{
				float plane_1[4]{};
				float plane_2[4]{};
				float plane_3[4]{};

				float v38;
				float v39;
				float v40;
				float v41;
				float v43;
				float v44;
				float v45;
				float v51;
				float v52;
				float v53;

				auto adjacent_side_offset = 0;
				auto edge_count = 0;

				if (brush_side >= 6)
				{
					const auto side = &brush->sides[brush_side - 6];
					adjacent_side_offset = side->firstAdjacentSideOffset;
					edge_count = side->edgeCount;

					const auto plane = side->plane;
					plane_1[0] = plane->normal[0];
					plane_1[1] = plane->normal[1];
					plane_1[2] = plane->normal[2];
					plane_1[3] = plane->dist;
				}
				else
				{
					const auto index = 3 * (brush_side & 1) + (brush_side >> 1);
					adjacent_side_offset = brush->firstAdjacentSideOffsets[0][index];
					edge_count = brush->edgeCount[0][index];

					plane_1[0] = axial_planes->planes[brush_side][0];
					plane_1[1] = axial_planes->planes[brush_side][1];
					plane_1[2] = axial_planes->planes[brush_side][2];
					plane_1[3] = axial_planes->planes[brush_side][3];
				}

				v51 = plane_1[3];
				v52 = plane_1[2];
				v53 = plane_1[0];

				if (edge_count < 3 || edge_count > max_verts)
				{
					out_winding->ptCount = 0;
					return;
				}

				const auto adjacent_side = &brush->baseAdjacentSide[adjacent_side_offset];
				const auto side_index = adjacent_side[edge_count - 1];

				if (adjacent_side[edge_count - 1] >= 6u)
				{
					const auto plane = brush->sides[(side_index - 6)].plane;
					plane_2[0] = plane->normal[0];
					plane_2[1] = plane->normal[1];
					plane_2[2] = plane->normal[2];
					plane_2[3] = plane->dist;
				}
				else
				{
					plane_2[0] = axial_planes->planes[side_index][0];
					plane_2[1] = axial_planes->planes[side_index][1];
					plane_2[2] = axial_planes->planes[side_index][2];
					plane_2[3] = axial_planes->planes[side_index][3];
				}

				out_winding->ptCount = 0;
				const auto points = out_winding->pts;
				auto current_point = 0;

				for (auto i = 0; i < edge_count; i++)
				{
					const auto current_side = adjacent_side[i];
					if (current_side >= 6u)
					{
						const auto plane = brush->sides[(current_side - 6)].plane;
						plane_3[0] = plane->normal[0];
						plane_3[1] = plane->normal[1];
						plane_3[2] = plane->normal[2];
						plane_3[3] = plane->dist;
					}
					else
					{
						plane_3[0] = axial_planes->planes[current_side][0];
						plane_3[1] = axial_planes->planes[current_side][1];
						plane_3[2] = axial_planes->planes[current_side][2];
						plane_3[3] = axial_planes->planes[current_side][3];
					}

					v38 = (plane_3[2] * plane_2[1]) - (plane_2[2] * plane_3[1]);
					v39 = (plane_1[2] * plane_3[1]) - (plane_1[1] * plane_3[2]);
					v40 = (plane_1[1] * plane_2[2]) - (plane_1[2] * plane_2[1]);
					v41 = (plane_2[0] * v39) + (plane_1[0] * v38) + (plane_3[0] * v40);

					const auto abs = std::fabs(v41);
					if (abs >= 0.001f)
					{
						points[current_point][0] = (((v39 * plane_2[3]) + (v38 * v51)) + (v40 * plane_3[3])) * (1.f / v41);
						points[current_point][1] = (((((v53 * plane_3[2]) - (plane_3[0] * v52)) * plane_2[3]) + (((plane_3[0] * plane_2[2]) - (plane_2[0] * plane_3[2])) * v51))
							+ (((plane_2[0] * v52) - (v53 * plane_2[2])) * plane_3[3]))
							* (1.f / v41);
						points[current_point][2] = (((((plane_3[0] * plane_1[1]) - (v53 * plane_3[1])) * plane_2[3]) + (((plane_2[0] * plane_3[1]) - (plane_3[0] * plane_2[1])) * v51))
							+ (((v53 * plane_2[1]) - (plane_2[0] * plane_1[1])) * plane_3[3]))
							* (1.f / v41);

						if (!current_point
							|| (v43 = points[(current_point - 1)][0],
								v44 = points[(current_point - 1)][1] - points[current_point][1],
								v45 = points[(current_point - 1)][2] - points[current_point][2],
								(((v44 * v44) + ((v43 - points[current_point][0]) * (v43 - points[current_point][0]))) + (v45 * v45)) >= 1.0))
						{
							current_point++;
						}

						plane_1[0] = v53;
					}

					plane_1[2] = v52;
					plane_2[0] = plane_3[0];
					plane_2[1] = plane_3[1];
					plane_2[2] = plane_3[2];
					plane_2[3] = plane_3[3];
				}

				out_winding->ptCount = current_point;
				const auto point_count = out_winding->ptCount;

				if (point_count > 1)
				{
					const auto pt_index = point_count - 1;

					float point[3]{};
					point[0] = out_winding->pts[0][0] - out_winding->pts[pt_index][0];
					point[1] = out_winding->pts[0][1] - out_winding->pts[pt_index][1];
					point[2] = out_winding->pts[0][2] - out_winding->pts[pt_index][2];

					if ((point[0] * point[0]) + (point[1] * point[1]) + (point[2] * point[2]) < 1.0)
					{
						out_winding->ptCount = pt_index;
					}
				}

				if (out_winding->ptCount < 3)
				{
					out_winding->ptCount = 0;
				}
			}

			axial_planes_t get_axial_planes(const Bounds* bounds)
			{
				axial_planes_t axial_planes{};
				axial_planes.planes[0][0] = -1.f;
				axial_planes.planes[0][1] = 0.f;
				axial_planes.planes[0][2] = 0.f;
				axial_planes.planes[0][3] = -1 * (bounds->midPoint[0] - bounds->halfSize[0]);

				axial_planes.planes[1][0] = 1.f;
				axial_planes.planes[1][1] = 0.f;
				axial_planes.planes[1][2] = 0.f;
				axial_planes.planes[1][3] = bounds->midPoint[0] + bounds->halfSize[0];

				axial_planes.planes[2][0] = 0.f;
				axial_planes.planes[2][1] = -1.f;
				axial_planes.planes[2][2] = 0.f;
				axial_planes.planes[2][3] = -1 * (bounds->midPoint[1] - bounds->halfSize[1]);

				axial_planes.planes[3][0] = 0.f;
				axial_planes.planes[3][1] = 1.f;
				axial_planes.planes[3][2] = 0.f;
				axial_planes.planes[3][3] = (bounds->midPoint[1] + bounds->halfSize[1]);

				axial_planes.planes[4][0] = 0.f;
				axial_planes.planes[4][1] = 0.f;
				axial_planes.planes[4][2] = -1.f;
				axial_planes.planes[4][3] = -1 * (bounds->midPoint[2] - bounds->halfSize[2]);

				axial_planes.planes[5][0] = 0.f;
				axial_planes.planes[5][1] = 0.f;
				axial_planes.planes[5][2] = 1.f;
				axial_planes.planes[5][3] = (bounds->midPoint[2] + bounds->halfSize[2]);

				return axial_planes;
			}

			void build_windings_for_brush(cplane_s* planes, const Bounds* bounds,
				const cbrush_t* brush, polygon_t* out_polys,
				unsigned int max_polys, float(*out_verts)[3], unsigned int max_verts)
			{
				auto point_count = 0;
				const auto axial_planes = get_axial_planes(bounds);

				for (auto side_index = 0; side_index < brush->numsides + 6; side_index++)
				{
					const auto out_poly = &out_polys[side_index];
					out_poly->pts = &out_verts[point_count];
					get_winding_for_brush_face(planes, bounds, brush, side_index, out_poly, max_verts - point_count, &axial_planes);
					point_count += out_poly->ptCount;
				}
			}

			std::vector<triangle_t> generate_brush_triangles(PhysGeomInfo* geom, std::vector<H1::dmFloat4>& vertices)
			{
				std::vector<triangle_t> triangles;

				assert(sizeof(cbrush_t) == sizeof(cbrushWrapper_t));
				auto brush = reinterpret_cast<cbrush_t*>(&geom->brushWrapper->brush);
				auto brush_bounds = geom->brushWrapper->bounds;

				const auto add_points = [&](int side_index, polygon_t* poly)
				{
					const auto base_index = static_cast<int>(vertices.size());

					for (auto i = 0u; i < poly->ptCount; i++)
					{
						H1::dmFloat4 vert{};
						vert.x = poly->pts[i][0];
						vert.y = poly->pts[i][1];
						vert.z = poly->pts[i][2];
						vertices.emplace_back(vert);
					}

					if (poly->ptCount == 3)
					{
						triangle_t triangle{};
						triangle.verts[0] = base_index + 0;
						triangle.verts[1] = base_index + 1;
						triangle.verts[2] = base_index + 2;
						triangles.emplace_back(triangle);
					}
					else if (poly->ptCount == 4)
					{
						triangle_t triangle_1{};
						triangle_1.verts[0] = base_index + 0;
						triangle_1.verts[1] = base_index + 1;
						triangle_1.verts[2] = base_index + 2;
						triangles.emplace_back(triangle_1);

						triangle_t triangle_2{};
						triangle_2.verts[0] = base_index + 2;
						triangle_2.verts[1] = base_index + 3;
						triangle_2.verts[2] = base_index + 0;
						triangles.emplace_back(triangle_2);
					}
					else
					{
						using point = std::array<float, 2>;
						std::vector<point> points;
						std::vector<std::vector<point>> polygon;

						for (auto i = 0u; i < poly->ptCount; i++)
						{
							point p{};
							p[0] = poly->pts[i][0];
							p[1] = poly->pts[i][1];

							points.push_back(p);
						}

						polygon.push_back(points);

						const auto indices = mapbox::earcut<int>(polygon);
						for (auto i = 0u; i < indices.size(); i += 3)
						{
							triangle_t triangle{};
							triangle.verts[0] = base_index + indices[i + 2];
							triangle.verts[1] = base_index + indices[i + 1];
							triangle.verts[2] = base_index + indices[i + 0];
							triangles.emplace_back(triangle);
						}
					}
				};

				constexpr auto max_verts = 0x1000;
				constexpr auto max_polys = 0x1000;
				float verts[max_verts][3]{};
				polygon_t polys[max_verts]{};

				build_windings_for_brush(geom->brushWrapper->planes, &brush_bounds, brush, polys, max_polys, verts, max_verts);
				for (auto side_index = 0; side_index < brush->numsides + 6; side_index++)
				{
					const auto poly = &polys[side_index];
					add_points(side_index, poly);
				}

				return triangles;
			}

			std::vector<triangle_t> generate_triangles(PhysGeomInfo* geom, std::vector<H1::dmFloat4>& vertices)
			{
				std::vector<triangle_t> triangles;
				std::unordered_set<int> triangle_hashes;

				auto triangle_index = 0;

				const auto add_triangle = [&](triangle_t& triangle)
				{
					const auto hash = hash_triangle(triangle, vertices);
					if (triangle_hashes.contains(hash))
					{
						return;
					}

					triangle_hashes.insert(hash);
					triangle.index = triangle_index++;
					triangles.emplace_back(triangle);
				};

				const auto brush_tris = generate_brush_triangles(geom, vertices);
				for (auto tri : brush_tris)
				{
					add_triangle(tri);
				}

				return triangles;
			}
		}

		struct dmPolytopeData
		{
			H1::dmFloat4* m_aVertices; // count: m_vertexCount
			H1::dmFloat4* m_aPlanes; // count: m_faceCount
			H1::dm_uint16* m_surfaceTypes; // count: m_faceCount // ALWAYS 0
			H1::dm_uint16* m_vertexMaterials; // count: m_vertexCount // ALWAYS 0
			H1::dmSubEdge* m_aSubEdges; // count: m_subEdgeCount
			H1::dm_uint8* m_aFaceSubEdges; // count: m_faceCount
			H1::dmFloat3 m_centroid;
			H1::dm_int32 m_vertexCount;
			H1::dm_int32 m_faceCount;
			H1::dm_int32 m_subEdgeCount;
			H1::dm_float32 m_volume;
			H1::dm_float32 m_area;
			H1::dmFloat3 unk1;
			H1::dmFloat3 unk2;
			int contents;
			int pad2;
		};

		void GetBoxVertices(const Bounds& bounds, vec3_t vertices[8]) 
		{
			// Extract midpoint and halfSize for readability
			const vec3_t& midPoint = bounds.midPoint;
			const vec3_t& halfSize = bounds.halfSize;

			// Calculate the 8 vertices of the box
			vertices[0][0] = midPoint[0] - halfSize[0];
			vertices[0][1] = midPoint[1] - halfSize[1];
			vertices[0][2] = midPoint[2] - halfSize[2];

			vertices[1][0] = midPoint[0] + halfSize[0];
			vertices[1][1] = midPoint[1] - halfSize[1];
			vertices[1][2] = midPoint[2] - halfSize[2];

			vertices[2][0] = midPoint[0] + halfSize[0];
			vertices[2][1] = midPoint[1] + halfSize[1];
			vertices[2][2] = midPoint[2] - halfSize[2];

			vertices[3][0] = midPoint[0] - halfSize[0];
			vertices[3][1] = midPoint[1] + halfSize[1];
			vertices[3][2] = midPoint[2] - halfSize[2];

			vertices[4][0] = midPoint[0] - halfSize[0];
			vertices[4][1] = midPoint[1] - halfSize[1];
			vertices[4][2] = midPoint[2] + halfSize[2];

			vertices[5][0] = midPoint[0] + halfSize[0];
			vertices[5][1] = midPoint[1] - halfSize[1];
			vertices[5][2] = midPoint[2] + halfSize[2];

			vertices[6][0] = midPoint[0] + halfSize[0];
			vertices[6][1] = midPoint[1] + halfSize[1];
			vertices[6][2] = midPoint[2] + halfSize[2];

			vertices[7][0] = midPoint[0] - halfSize[0];
			vertices[7][1] = midPoint[1] + halfSize[1];
			vertices[7][2] = midPoint[2] + halfSize[2];
		}

		void GenerateBox(PhysGeomInfo* geom, H1::PhysGeomInfo* h1_geom, allocator& allocator)
		{
			h1_geom->data = allocator.allocate<H1::dmPolytopeData>();
			auto* data = reinterpret_cast<dmPolytopeData*>(h1_geom->data);

			vec3_t vertices[8]{};
			GetBoxVertices(geom->bounds, &vertices[0]);

			const auto vertexCount = 8;
			const auto indicesCount = data->m_vertexCount * 3;
			const auto facesCount = indicesCount / 4;
			data->m_vertexCount = vertexCount;
			data->m_subEdgeCount = indicesCount;
			data->m_faceCount = facesCount;

			memcpy(&h1_geom->data->m_centroid, &geom->bounds, sizeof(float[3]));
		}

		void GenerateCylinder(PhysGeomInfo* geom, H1::PhysGeomInfo* h1_geom, allocator& allocator)
		{

		}

		void GenerateCapsule(PhysGeomInfo* geom, H1::PhysGeomInfo* h1_geom, allocator& allocator)
		{

		}

		void GenerateBrush(PhysGeomInfo* geom, H1::PhysGeomInfo* h1_geom, allocator& allocator)
		{
			h1_geom->data = allocator.allocate<H1::dmPolytopeData>();
			auto* data = reinterpret_cast<dmPolytopeData*>(h1_geom->data);

			data->m_subEdgeCount = geom->brushWrapper->totalEdgeCount;

			std::vector<H1::dmFloat4> vertices;
			auto triangles = generate_triangles(geom, vertices);

			ZONETOOL_INFO("total tris: %zu, verts: %zu", triangles.size(), vertices.size());

			data->m_vertexCount = static_cast<int>(vertices.size());
			data->m_aVertices = allocator.allocate<H1::dmFloat4>(data->m_vertexCount);
			std::memcpy(data->m_aVertices, vertices.data(), vertices.size() * sizeof(H1::dmFloat4));

			data->m_faceCount = static_cast<int>(triangles.size());
			data->m_aPlanes = allocator.allocate<H1::dmFloat4>(data->m_faceCount);

			for (const auto& tri : triangles)
			{
				data->m_aPlanes[tri.index].x = tri.verts[2];
				data->m_aPlanes[tri.index].y = tri.verts[1];
				data->m_aPlanes[tri.index].z = tri.verts[0];
				data->m_aPlanes[tri.index].w = 0;
			}
		}

		H1::PhysCollmap* GenerateH1Asset(PhysCollmap* asset, allocator& allocator)
		{
			ZONETOOL_INFO("Generating PhysCollmap %s", asset->name);

			auto* h1_asset = allocator.allocate<H1::PhysCollmap>();

			h1_asset->name = asset->name;
			
			h1_asset->count = asset->count;
			memcpy(&h1_asset->mass, &asset->mass, sizeof(PhysMass));
			memcpy(&h1_asset->bounds, &asset->bounds, sizeof(PhysMass));

			h1_asset->geoms = allocator.allocate<H1::PhysGeomInfo>(h1_asset->count);
			for (auto i = 0; i < h1_asset->count; i++)
			{
				auto* geom = &asset->geoms[i];
				auto* h1_geom = &h1_asset->geoms[i];

				auto type = geom->type;
				switch(type)
				{
				case PHYS_GEOM_NONE:
					break; // sphere...
				case PHYS_GEOM_BOX:
					GenerateBox(geom, h1_geom, allocator);
					break;
				case PHYS_GEOM_CYLINDER:
					GenerateCylinder(geom, h1_geom, allocator);
					break;
				case PHYS_GEOM_CAPSULE:
					GenerateCapsule(geom, h1_geom, allocator);
					break;
				case PHYS_GEOM_BRUSHMODEL:
				case PHYS_GEOM_BRUSH:
					GenerateBrush(geom, h1_geom, allocator);
					break;
				default:
					__debugbreak();
					break;
				}

				//h1_geom->data = allocator.allocate<H1::dmPolytopeData>(1);
				if (h1_geom->data)
				{
					h1_geom->data->contents = -1;
					memcpy(&h1_geom->data->m_centroid, &h1_asset->mass.centerOfMass, sizeof(float[3]));
				}

				auto x = 0;
			}

			ZONETOOL_INFO("PhysCollmap %s generated!", asset->name);

			return h1_asset;
		}

		H1::PhysCollmap* convert(PhysCollmap* asset, allocator& allocator)
		{
			// generate h1 asset
			return GenerateH1Asset(asset, allocator);
		}
	}
}