#include "stdafx.hpp"
#include "IW4/Assets/RawFile.hpp"

namespace ZoneTool
{
	namespace IW3
	{
		void IRawFile::dump(RawFile* asset)
		{
			// dump rawfile
			IW4::IRawFile::dump(reinterpret_cast<IW4::RawFile*>(asset));
		}
	}
}