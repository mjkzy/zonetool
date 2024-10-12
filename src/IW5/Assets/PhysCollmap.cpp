#include "stdafx.hpp"

#include "Dumper/H1/Assets/PhysCollmap.hpp"
//#include "Dumper/IW6/Assets/PhysCollmap.hpp"
//#include "Dumper/S1/Assets/PhysCollmap.hpp"

namespace ZoneTool::IW5
{
	void IPhysCollmap::dump(PhysCollmap* asset)
	{
		if (zonetool::dumping_target == zonetool::dump_target::h1)
		{
			return H1Dumper::dump(asset);
		}
		else if (zonetool::dumping_target == zonetool::dump_target::iw6)
		{
			//return IW6Dumper::dump(asset);
		}
		else if (zonetool::dumping_target == zonetool::dump_target::s1)
		{
			//return S1Dumper::dump(asset);
		}
	}
}