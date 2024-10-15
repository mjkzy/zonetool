#pragma once

namespace ZoneTool::H1
{
	class IPathData
	{
	public:
		static void dump(PathData* asset, const std::function<const char* (std::uint16_t)>& SL_ConvertToString);
	};
}
