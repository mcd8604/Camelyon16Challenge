#include "Minimap.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace tepis;
using namespace std;
using namespace cv;

Minimap::Minimap()
{
}


Minimap::~Minimap()
{
}


int main(int argc, char **argv) {
	namedWindow("Minimap", WINDOW_KEEPRATIO);

	TepisClient tepisClient("https://tepis.ctmm-trait.nl/");
	// TODO externalize credentials
	tepisClient.authenticate("demaurm", "kJJC72JF2!jf@");
	string imageID = "1.3.46.670589.45.1.1.118986034794496.1.4884.1448292561733.2";

	// Get Metadata
	ImageMetadata imageMetadata;
	imageMetadata = tepisClient.getImageMetadata(imageID);
	PixelMetadata pixelMetadata = imageMetadata.getPixelMetadata();
	vector<PixelLevelMetadata> &levels = pixelMetadata.getLevels();

	// Get ImagePixelData
	vector<unsigned char> imagePixelData = tepisClient.getImagePixelData(imageID, 0, 0, 512, 1024, 8, Unit::PIXEL);
	Mat image = imdecode(imagePixelData, CV_LOAD_IMAGE_COLOR);

	// PreProcess Image
	Mat grayImage;
	grayImage.create(image.size(), image.type());
	cvtColor(image, grayImage, COLOR_BGR2GRAY);
	//cvtColor(imagePixelData, grayImage, CV_RGB2GRAY);
	int thresholdFeature = 160;
	threshold(grayImage, grayImage, thresholdFeature, 255, 0);

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
