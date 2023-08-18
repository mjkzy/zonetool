#include "stdafx.hpp"
#include "H1/Assets/Sound.hpp"

#include "Converter/H1/Assets/Sound.hpp"

namespace ZoneTool::IW5
{
	void ISound::dump(snd_alias_list_t* asset, ZoneMemory* mem, const std::function<std::string(const char* filename)>& get_streamed_sound_data)
	{
		// generate h1 asset
		auto* h1_asset = Converter::convert(asset, mem, get_streamed_sound_data);

		// dump lightdef
		H1::ISound::dump(h1_asset);
	}
}