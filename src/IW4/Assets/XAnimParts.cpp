#include "stdafx.hpp"
#include "IW5/Assets/XAnimParts.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		void IXAnimParts::dump(XAnimParts* asset, const std::function<const char* (uint16_t)>& convertToString)
		{
			// dump anims
			IW5::IXAnimParts::dump(reinterpret_cast<IW5::XAnimParts*>(asset), convertToString);
		}
	}
}