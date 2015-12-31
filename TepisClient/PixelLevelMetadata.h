#pragma once

#include <string>

namespace tepis {

	class PixelLevelMetadata
	{
	public:
		PixelLevelMetadata(
			int level,
			std::string pixelSize,
			std::string physicalOrigin,
			std::string physicalSpacing,
			double scanFactor,
			bool isNativeLevel,
			bool isLossyCompressed,
			int lossyCompressionRatio,
			std::string derivationDescription,
			std::string tileSize);
		~PixelLevelMetadata();
		int getLevel();
		std::string getPixelSize();
		std::string getPhysicalOrigin();
		std::string getPhysicalSpacing();
		double getScanFactor();
		bool getIsNativeLevel();
		bool getIsLossyCompressed();
		int getLossyCompressionRatio();
		std::string getDerivationDescription();
		std::string getTileSize();
	private:
		int level;
		std::string pixelSize;
		std::string physicalOrigin;
		std::string physicalSpacing;
		double scanFactor;
		bool isNativeLevel;
		bool isLossyCompressed;
		int lossyCompressionRatio;
		std::string derivationDescription;
		std::string tileSize;
	};

}