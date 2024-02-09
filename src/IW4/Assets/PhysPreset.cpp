#include "stdafx.hpp"
#include "IW5/Assets/PhysPreset.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		void IPhysPreset::dump(PhysPreset* asset)
		{
			// dump asset
			IW5::IPhysPreset::dump(reinterpret_cast<IW5::PhysPreset*>(asset));
		}
	}
}