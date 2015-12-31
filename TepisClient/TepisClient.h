#pragma once	

#include "ImageMetadata.h"
#include <string>
#include <cpprest/http_client.h>

namespace tepis
{

	enum Unit {
		MM,
		UM,
		PIXEL
	};

	inline const char* toString(Unit v)
	{
		switch (v)
		{
		case MM:	return "mm";
		case UM:	return "um";
		case PIXEL:	return "pixel";
		default:	return "[Unknown Unit]";
		}
	}
						
	class TepisClient						
	{										
	public:									
		TepisClient(const std::string &serverAddress);
		~TepisClient();
		void authenticate(const std::string &username, const std::string &password);
		ImageMetadata getImageMetadata(const std::string &imageID);
		pplx::task<ImageMetadata> getImageMetadata_Async(const std::string &imageID);
		void getAssociatedImage(const std::string &imageID);
		pplx::task<std::vector<unsigned char>> getImagePixelData_Async(const std::string &imageID, float x, float y, float width, float height, int level, Unit unit);
		std::vector<unsigned char> getImagePixelData(const std::string &imageID, float x, float y, float width, float height, int level, Unit unit);
		void getTiledImagePixelData(const std::string &imageID);
	private:
		web::http::client::http_client httpClient;
		//utility::string_t authStr;
		utility::string_t cookie;
		pplx::task<web::http::http_response> getImageMetadataHTTP(const std::string &imageID);
		ImageMetadata parseMetadataResponseXML(std::istringstream &s);
		ImageMetadata parseMetadataResponseJSON(web::json::value json);
		pplx::task<web::http::http_response> getImagePixelData_HTTP(const std::string &imageID, float x, float y, float width, float height, int level, Unit unit);
	};
										
}