#pragma once

namespace ZoneTool
{
	namespace IW4
	{
		const char* SL_ConvertToString(std::uint16_t index);

		class IXAnimParts
		{
		public:
			static void dump(XAnimParts* asset, const std::function<const char* (uint16_t)>& convertToString = SL_ConvertToString);
		};
	}
}