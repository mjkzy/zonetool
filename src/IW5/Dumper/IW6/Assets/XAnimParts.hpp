#pragma once

namespace ZoneTool::IW5
{
	namespace IW6Dumper
	{
		void dump(XAnimParts* asset, const std::function<const char* (uint16_t)>& convertToString);
	}
}