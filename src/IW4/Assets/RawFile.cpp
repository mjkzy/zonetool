#include "stdafx.hpp"
#include "IW5/Assets/RawFile.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		void IRawFile::dump(RawFile* asset)
		{
			// dump rawfile
			IW5::IRawFile::dump(reinterpret_cast<IW5::RawFile*>(asset));
		}
	}
}