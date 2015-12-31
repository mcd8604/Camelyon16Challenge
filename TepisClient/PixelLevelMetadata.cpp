#include "PixelLevelMetadata.h"

using namespace tepis;
using namespace std;

PixelLevelMetadata::PixelLevelMetadata(
	int level,
	string pixelSize,
	string physicalOrigin,
	string physicalSpacing,
	double scanFactor,
	bool isNativeLevel,
	bool isLossyCompressed,
	int lossyCompressionRatio,
	string derivationDescription,
	string tileSize) :
	level(level),
	pixelSize(pixelSize),
	physicalOrigin(physicalOrigin),
	physicalSpacing(physicalSpacing),
	scanFactor(scanFactor),
	isNativeLevel(isNativeLevel),
	isLossyCompressed(isLossyCompressed),
	lossyCompressionRatio(lossyCompressionRatio),
	derivationDescription(derivationDescription),
	tileSize(tileSize)
{
}


PixelLevelMetadata::~PixelLevelMetadata()
{
}

int PixelLevelMetadata::getLevel() { return level;  }
string PixelLevelMetadata::getPixelSize() { return pixelSize;  }
string PixelLevelMetadata::getPhysicalOrigin() { return physicalOrigin;  }
string PixelLevelMetadata::getPhysicalSpacing() { return physicalSpacing;  }
double PixelLevelMetadata::getScanFactor() { return scanFactor;  }
bool PixelLevelMetadata::getIsNativeLevel() { return isNativeLevel; }
bool PixelLevelMetadata::getIsLossyCompressed() { return isLossyCompressed;  }
int PixelLevelMetadata::getLossyCompressionRatio() { return lossyCompressionRatio; }
string PixelLevelMetadata::getDerivationDescription() { return derivationDescription; }
string PixelLevelMetadata::getTileSize() { return tileSize;  }