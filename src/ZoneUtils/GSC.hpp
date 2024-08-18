#pragma once

#include <xsk/gsc/engine/iw5_pc.hpp>

namespace gsc
{
	namespace iw5
	{
		extern std::unique_ptr<xsk::gsc::iw5_pc::context> gsc_ctx;
	}
}