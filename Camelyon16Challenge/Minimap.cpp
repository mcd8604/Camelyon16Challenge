#include "Minimap.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <boost/lexical_cast.hpp>

using namespace tepis;
using namespace std;
using namespace cv;

Minimap::Minimap()
{
}


Minimap::~Minimap()
{
}

Size getSizeFromLevelMeta(PixelLevelMetadata level) {
	string pxSize = level.getPixelSize();
	int commaLoc = pxSize.find(",");
	string pxWidth = pxSize.substr(0, commaLoc);
	string pxHeight = pxSize.substr(commaLoc);
	int width = boost::lexical_cast<int>(pxWidth);
	int height = boost::lexical_cast<int>(pxHeight);
}

int main(int argc, char **argv) {
	namedWindow("Minimap", WINDOW_KEEPRATIO);

	TepisClient tepisClient("https://tepis.ctmm-trait.nl/");
	// TODO externalize credentials
	tepisClient.authenticate("demaurm", "kJJC72JF2!jf@");
	string imageID = "1.3.46.670589.45.1.1.118986034794496.1.4884.1448292561733.2";
	int miniLevel = 8; // TODO parameterize miniLevel

	// Get Metadata
	ImageMetadata imageMetadata;
	imageMetadata = tepisClient.getImageMetadata(imageID);
	PixelMetadata pixelMetadata = imageMetadata.getPixelMetadata();
	vector<PixelLevelMetadata> &levels = pixelMetadata.getLevels();
	Size nativeMetaSize = getSizeFromLevelMeta(levels[0]);
	Size miniMetaSize = getSizeFromLevelMeta(levels[miniLevel]);

	// Get ImagePixelData
	// TODO check image for origin offsets instead of hardcoding (x=0, y=0) 
	vector<unsigned char> imagePixelData = tepisClient.getImagePixelData(imageID, 0, 0, miniMetaSize.width, miniMetaSize.height, miniLevel, Unit::PIXEL);
	Mat image = imdecode(imagePixelData, CV_LOAD_IMAGE_COLOR);

	// Verify metadata pixel size matches image size
	if (image.rows != miniMetaSize.height || image.cols != miniMetaSize.width) {
		// TODO throw error
	}

	// PreProcess Image
	Mat grayImage;
	//grayImage.create(image.size(), image.type());
	cvtColor(image, grayImage, COLOR_BGR2GRAY);
	// TODO choose:
	// 1 - parameterize constant value threshold
	// 2 - calculate variable value based on stain metadata
	// 3 - switch to adaptive threshold
	int thresholdFeature = 160;
	blur(grayImage, grayImage, Size(5, 5));
	threshold(grayImage, grayImage, thresholdFeature, 255, 0);

	// TODO Get tile size
	int tileWidth = 512;
	int tileHeight = 512;

	int numTilesX = nativeMetaSize.width / tileWidth;
	int numTilesY = nativeMetaSize.height / tileHeight;

	// TODO maybe add a check for remainder pixels

	// Iterate over minimap by using a submatrix and adjusting it
	int subMatWidth = tileWidth / pow(2, miniLevel);
	int subMatHeight = tileHeight / pow(2, miniLevel);
	Rect r(0, 0, subMatWidth, subMatHeight);
	Mat subMat = grayImage(r);
	for (int y = 0; y < numTilesY; y++) {
		for (int x = 0; x < numTilesX; x++) {
			// TODO sum intensities in subMat
			// TODO Check if tissue exists in tile
			if (true) {
				// Retrieve native resolution for tile
				vector<unsigned char> tilePixelData = tepisClient.getImagePixelData(imageID, x * tileWidth, y * tileHeight, tileWidth, tileHeight, 0, Unit::PIXEL);
				Mat tile = imdecode(tilePixelData, CV_LOAD_IMAGE_COLOR);

				/// Construct features for tile
				// TODO Save tile image to file
				//write(tile, "test");

				// TODO Invoke CellProfiler

				// TODO Store features for tile
			}
			subMat.adjustROI(0, 0, 0, subMatWidth);
		}
		subMat.adjustROI(0, subMatHeight, 0, 0);
	}

	bool s = true;
	while (s || waitKey(0) != 27) //esc
	{
		s = false;
		imshow("Minimap", grayImage);
	}

	// Wait indefinitely
	//pplx::wait(concurrency::event::timeout_infinite);

	// Cleanup all windows
	destroyAllWindows();

	return 0;
}
