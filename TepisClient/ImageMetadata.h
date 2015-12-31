#pragma once

#include "PixelMetadata.h"
#include "ScannerMetadata.h"
#include "StainMetadata.h"
#include "PrimaryAnatomicStructure.h"

namespace tepis
{

	class ImageMetadata
	{
	public:
		ImageMetadata();
		~ImageMetadata();
		ScannerMetadata &getScannerMetadata();
		PrimaryAnatomicStructure &getPrimaryAnatomicStructure();
		StainMetadata &getStainMetadata();
		PixelMetadata &getPixelMetadata();
	private:
		ScannerMetadata scannerMetadata;
		PrimaryAnatomicStructure primaryAnatomicStructure;
		StainMetadata stainMetadata;
		PixelMetadata pixelMetadata;
	};

}