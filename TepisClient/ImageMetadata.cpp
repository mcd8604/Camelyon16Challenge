#include "ImageMetadata.h"

using namespace tepis;

ImageMetadata::ImageMetadata()
{
}


ImageMetadata::~ImageMetadata()
{
}

ScannerMetadata &ImageMetadata::getScannerMetadata()
{
	return scannerMetadata;
}

PrimaryAnatomicStructure &ImageMetadata::getPrimaryAnatomicStructure()
{
	return primaryAnatomicStructure;
}

StainMetadata &ImageMetadata::getStainMetadata()
{
	return stainMetadata;
}

PixelMetadata &ImageMetadata::getPixelMetadata()
{
	return pixelMetadata;
}
