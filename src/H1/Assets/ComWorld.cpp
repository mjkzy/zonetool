#include "stdafx.hpp"

#define DUMP_JSON

namespace ZoneTool::H1
{
#define DUMP_STRING(__field__) \
	static_assert(std::is_same_v<decltype(asset->__field__), const char PTR64>, "Field is not of type const char*"); \
	asset->__field__ ? data[#__field__] = asset->__field__ : data[#__field__] = "";

#define DUMP_FIELD(__field__) \
	data[#__field__] = asset->__field__;

#define DUMP_FIELD_ARR(__field__, __size__) \
	for (auto idx##__field__ = 0u; idx##__field__ < (unsigned int)__size__; idx##__field__++) \
	{ \
		data[#__field__][idx##__field__] = asset->__field__[idx##__field__]; \
	}

	void dump(ComPrimaryLight* asset, ordered_json& data)
	{
		DUMP_FIELD(type);
		DUMP_FIELD(canUseShadowMap);
		DUMP_FIELD(physicallyBased);
		DUMP_FIELD(exponent);
		DUMP_FIELD(lightingState);
		DUMP_FIELD_ARR(color, 3);
		DUMP_FIELD_ARR(dir, 3);
		DUMP_FIELD_ARR(up, 3);
		DUMP_FIELD_ARR(origin, 3);
		DUMP_FIELD_ARR(fadeOffset, 2);
		DUMP_FIELD(bulbRadius);
		DUMP_FIELD_ARR(bulbLength, 3);
		DUMP_FIELD(radius);
		DUMP_FIELD(cosHalfFovOuter);
		DUMP_FIELD(cosHalfFovInner);
		DUMP_FIELD(cosHalfFovExpanded);
		DUMP_FIELD(rotationLimit);
		DUMP_FIELD(translationLimit);
		DUMP_FIELD(cucRotationOffsetRad);
		DUMP_FIELD(cucRotationSpeedRad);
		DUMP_FIELD_ARR(cucScrollVector, 2);
		DUMP_FIELD_ARR(cucScaleVector, 2);
		DUMP_FIELD_ARR(cucTransVector, 2);
		DUMP_STRING(defName);
	}

	void dump(ComPrimaryLightEnv* asset, ordered_json& data)
	{
		DUMP_FIELD_ARR(primaryLightIndices, 4);
		DUMP_FIELD(numIndices);
	}

	void dump_json(ComWorld* asset)
	{
		const auto path = asset->name + ".commap"s + ".json";
		auto file = filesystem::file(path);
		file.open("wb");

		ordered_json data;

		DUMP_FIELD(isInUse);

		data["primaryLight"] = {};
		for (auto i = 0u; i < asset->primaryLightCount; i++)
		{
			dump(&asset->primaryLights[i], data["primaryLight"][i]);
		}

		data["primaryLightEnv"] = {};
		for (auto i = 0u; i < asset->primaryLightEnvCount; i++)
		{
			dump(&asset->primaryLightEnvs[i], data["primaryLightEnv"][i]);
		}

		auto str = data.dump(4);
		data.clear();
		file.write(str);
		file.close();
	}


	void IComWorld::dump(ComWorld* asset)
	{
#ifdef DUMP_JSON
		dump_json(asset);
		return;
#else
		const auto path = asset->name + ".commap"s;

		assetmanager::dumper write;
		if (!write.open(path))
		{
			return;
		}

		write.dump_single(asset);
		write.dump_string(asset->name);

		write.dump_array(asset->primaryLights, asset->primaryLightCount);
		for (unsigned int i = 0; i < asset->primaryLightCount; i++)
		{
			write.dump_string(asset->primaryLights[i].defName);
		}
		write.dump_array(asset->primaryLightEnvs, asset->primaryLightEnvCount);

		write.close();
#endif
	}
}