#include "stdafx.hpp"
#include "H1/Assets/GfxImage.hpp"

namespace ZoneTool
{
	namespace IW5
	{
		typedef enum _D3DFORMAT 
		{
			D3DFMT_UNKNOWN = 0,

			D3DFMT_R8G8B8 = 20,
			D3DFMT_A8R8G8B8 = 21,
			D3DFMT_X8R8G8B8 = 22,
			D3DFMT_R5G6B5 = 23,
			D3DFMT_X1R5G5B5 = 24,
			D3DFMT_A1R5G5B5 = 25,
			D3DFMT_A4R4G4B4 = 26,
			D3DFMT_R3G3B2 = 27,
			D3DFMT_A8 = 28,
			D3DFMT_A8R3G3B2 = 29,
			D3DFMT_X4R4G4B4 = 30,
			D3DFMT_A2B10G10R10 = 31,
			D3DFMT_A8B8G8R8 = 32,
			D3DFMT_X8B8G8R8 = 33,
			D3DFMT_G16R16 = 34,
			D3DFMT_A2R10G10B10 = 35,
			D3DFMT_A16B16G16R16 = 36,

			D3DFMT_A8P8 = 40,
			D3DFMT_P8 = 41,

			D3DFMT_L8 = 50,
			D3DFMT_A8L8 = 51,
			D3DFMT_A4L4 = 52,

			D3DFMT_V8U8 = 60,
			D3DFMT_L6V5U5 = 61,
			D3DFMT_X8L8V8U8 = 62,
			D3DFMT_Q8W8V8U8 = 63,
			D3DFMT_V16U16 = 64,
			D3DFMT_A2W10V10U10 = 67,

			//D3DFMT_UYVY = MAKEFOURCC('U', 'Y', 'V', 'Y'),
			//D3DFMT_R8G8_B8G8 = MAKEFOURCC('R', 'G', 'B', 'G'),
			//D3DFMT_YUY2 = MAKEFOURCC('Y', 'U', 'Y', '2'),
			//D3DFMT_G8R8_G8B8 = MAKEFOURCC('G', 'R', 'G', 'B'),
			//D3DFMT_DXT1 = MAKEFOURCC('D', 'X', 'T', '1'),
			//D3DFMT_DXT2 = MAKEFOURCC('D', 'X', 'T', '2'),
			//D3DFMT_DXT3 = MAKEFOURCC('D', 'X', 'T', '3'),
			//D3DFMT_DXT4 = MAKEFOURCC('D', 'X', 'T', '4'),
			//D3DFMT_DXT5 = MAKEFOURCC('D', 'X', 'T', '5'),

			D3DFMT_D16_LOCKABLE = 70,
			D3DFMT_D32 = 71,
			D3DFMT_D15S1 = 73,
			D3DFMT_D24S8 = 75,
			D3DFMT_D24X8 = 77,
			D3DFMT_D24X4S4 = 79,
			D3DFMT_D16 = 80,

			D3DFMT_D32F_LOCKABLE = 82,
			D3DFMT_D24FS8 = 83,

#if !defined(D3D_DISABLE_9EX)
			D3DFMT_D32_LOCKABLE = 84,
			D3DFMT_S8_LOCKABLE = 85,
#endif // !D3D_DISABLE_9EX

			D3DFMT_L16 = 81,

			D3DFMT_VERTEXDATA = 100,
			D3DFMT_INDEX16 = 101,
			D3DFMT_INDEX32 = 102,

			D3DFMT_Q16W16V16U16 = 110,

			//D3DFMT_MULTI2_ARGB8 = MAKEFOURCC('M', 'E', 'T', '1'),

			D3DFMT_R16F = 111,
			D3DFMT_G16R16F = 112,
			D3DFMT_A16B16G16R16F = 113,

			D3DFMT_R32F = 114,
			D3DFMT_G32R32F = 115,
			D3DFMT_A32B32G32R32F = 116,

			D3DFMT_CxV8U8 = 117,

#if !defined(D3D_DISABLE_9EX)
			D3DFMT_A1 = 118,
			D3DFMT_A2B10G10R10_XR_BIAS = 119,
			D3DFMT_BINARYBUFFER = 199,
#endif // !D3D_DISABLE_9EX

			D3DFMT_FORCE_DWORD = 0x7fffffff
		} D3DFORMAT;

		std::unordered_map<D3DFORMAT, DXGI_FORMAT> d3d_dxgi_map =
		{
			{D3DFMT_A8R8G8B8, DXGI_FORMAT_R8G8B8A8_UNORM},
			{D3DFMT_L8, DXGI_FORMAT_R8_UNORM},
		};

		DXGI_FORMAT get_d3d_to_dxgi(std::uint8_t d3d)
		{
			if (d3d_dxgi_map.find((D3DFORMAT)d3d) != d3d_dxgi_map.end())
			{
				return d3d_dxgi_map[(D3DFORMAT)d3d];
			}
			return DXGI_FORMAT_UNKNOWN;
		}

		H1::GfxImage* GenerateH1GfxImage(GfxImage* asset, ZoneMemory* mem)
		{
			// allocate H1 GfxImage structure
			const auto h1_asset = mem->Alloc<H1::GfxImage>();

			h1_asset->name = asset->name;
			h1_asset->imageFormat = get_d3d_to_dxgi(asset->texture->format); //h1_asset->imageFormat = MFMapDX9FormatToDXGIFormat(asset->texture->format);
			h1_asset->mapType = static_cast<H1::MapType>(asset->mapType);
			h1_asset->sematic = asset->semantic;
			h1_asset->category = asset->category;
			h1_asset->flags = asset->flags;
			h1_asset->dataLen1 = asset->texture->dataSize;
			h1_asset->dataLen2 = asset->texture->dataSize;
			h1_asset->width = asset->width;
			h1_asset->height = asset->height;
			h1_asset->depth = asset->depth;
			h1_asset->numElements = asset->texture->levelCount;
			h1_asset->levelCount = asset->mapType == 5 ? 7 : 1;
			h1_asset->streamed = false;
			h1_asset->pixelData = reinterpret_cast<unsigned char*>(&asset->texture->texture);

			if (h1_asset->imageFormat == DXGI_FORMAT_UNKNOWN)
			{
				ZONETOOL_INFO("Possible DXGIFORMAT: %d", MFMapDX9FormatToDXGIFormat(asset->texture->format));
				ZONETOOL_FATAL("Unknown DXGIFORMAT for image \"%s\" (%d)", asset->name, asset->texture->format);
			}

			return h1_asset;
		}

		void IGfxImage::dump(GfxImage* asset, ZoneMemory* mem)
		{
			std::string name = asset->name;

			bool isMapImage = (name.size() >= 6)
				? ((name.substr(0, 6) == "*light" || name.substr(0, 6) == "*refle" ||
					name == "$outdoor")
					? true
					: false)
				: false;

			if (!isMapImage) return;

			if (!asset->texture->dataSize)
			{
				ZONETOOL_INFO("Not dumping image %s", asset->name);
				return;
			}

			// generate h1 gfximage
			auto h1_asset = GenerateH1GfxImage(asset, mem);

			// dump h1 gfximage
			H1::IGfxImage::dump(h1_asset);
		}
	}
}