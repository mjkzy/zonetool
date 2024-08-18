#include "stdafx.hpp"
#include "GSC.hpp"

namespace gsc
{
	namespace iw5
	{
		std::unique_ptr<xsk::gsc::iw5_pc::context> gsc_ctx = std::make_unique<xsk::gsc::iw5_pc::context>();
	}
}