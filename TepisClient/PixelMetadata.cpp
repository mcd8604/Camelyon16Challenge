#include "PixelMetadata.h"

using namespace tepis;
using namespace std;

PixelMetadata::PixelMetadata()
{
}


PixelMetadata::~PixelMetadata()
{
}

vector<PixelLevelMetadata> &PixelMetadata::getLevels()
{
	return levels;
}
