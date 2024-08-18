#pragma once

namespace compression
{
	namespace zlib
	{
		std::string compress(const std::string& data);
		std::string decompress(const std::string& data);
	}
}