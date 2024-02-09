#include "stdafx.hpp"
#include "IW5/Assets/XModel.hpp"

namespace ZoneTool
{
	namespace IW4
	{
		void IXModel::dump(XModel* asset, const std::function<const char* (uint16_t)>& convertToString)
		{
			allocator allocator;
			auto* iw5_model = allocator.allocate<IW5::XModel>();
			memcpy(iw5_model, asset, sizeof(XModel));
			iw5_model->quantization = 0;

			// dump model
			IW5::IXModel::dump(iw5_model, convertToString);
		}
	}
}