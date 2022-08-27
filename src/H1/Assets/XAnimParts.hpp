#pragma once

namespace ZoneTool
{
	namespace H1
	{
		class IXAnimParts
		{
		public:
			static void dump(XAnimParts* asset, const std::function<const char* (std::uint16_t)>& SL_ConvertToString);
		};
	}
}