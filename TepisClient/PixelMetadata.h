#pragma once

#include "PixelLevelMetadata.h"
#include <vector>

namespace tepis
{

	class PixelMetadata
	{
	public:
		PixelMetadata();
		~PixelMetadata();
		std::vector<PixelLevelMetadata> &getLevels();
	private:
		std::vector<PixelLevelMetadata> levels;
	};

}