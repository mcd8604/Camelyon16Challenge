#include "TepisClient.h"
#include "cpprest\details\http_helpers.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

using namespace tepis;
using namespace std;
using namespace utility;
using namespace utility::details;
using namespace utility::conversions;
using namespace web;
using namespace web::http;
using namespace web::http::details;
using namespace web::json;
using namespace boost::property_tree;
using namespace pplx;

TepisClient::TepisClient(const std::string &serverAddress) :
	httpClient(uri(to_string_t(serverAddress)))
{
}


TepisClient::~TepisClient()
{
}


void TepisClient::authenticate(const std::string &username, const std::string &password)
{
	uri_builder uriBuilder = uri_builder(U("/AccessService")).append_path(U("/Login"));
	http_request request(methods::POST);
	request.set_request_uri(uriBuilder.to_string());
	string_t body = U("username=") + to_string_t(username) + U("&password=") + to_string_t(password);
	request.set_body(body, mime_types::application_x_www_form_urlencoded);
	task<http_response> responseTask = httpClient.request(request);
	// Force wait for auth
	http_response response = responseTask.get();
	int status_code = response.status_code();
	printf("Authenticate response status code:%u\n", status_code);
	if (status_code == status_codes::OK) {
		http_headers &headers = response.headers();
		cookie = headers.operator[](U("Set-Cookie"));
	}
}

task<http_response> TepisClient::getImageMetadataHTTP(const std::string &imageID) {
	// Create a request to the tEPIS ImageService to get image metadata
	uri_builder uriBuilder = uri_builder(U("/ImageService")).append_path(U("/image")).append_path(to_string_t(imageID)).append_path(U("/metadata"));
	http_request request(methods::GET);
	request.set_request_uri(uriBuilder.to_string());

// NOTE: Service only returns XML
//	request.set_request_uri(uriBuilder.to_string());
//	request.headers().add(header_names::accept, mime_types::application_json);

	// Invoke the request
	return httpClient.request(request);
}

ImageMetadata TepisClient::getImageMetadata(const std::string &imageID) {
	return parseMetadataResponseXML(istringstream(to_utf8string(getImageMetadataHTTP(imageID).get().extract_string().get())));
}

task<ImageMetadata> TepisClient::getImageMetadata_Async(const std::string &imageID)
{
	// Handle the response: extract and parse the JSON to metadata objects
	return getImageMetadataHTTP(imageID).then([this](http_response response) {
		printf("ImageMetadata response status code:%u\n", response.status_code());
		//printf("Content Type:%s\n", response.headers().content_type().c_str());
		http_headers headers = response.headers();
		string_t contentType = headers.content_type();

		ImageMetadata imageMetadata;
		
		//int isJSON = mime_types::application_json.compare(contentType);
		//int isXML = mime_types::application_xml.compare(contentType);
		//str_icmp(mime_types::application_xml, contentType);
		//if (isJSON) {
		if(false) {
			value json = response.extract_json().get();
			imageMetadata = parseMetadataResponseJSON(json);
		// } else if(isXML) {
		} else {
			const string_t &s = response.extract_string().get();
			imageMetadata = parseMetadataResponseXML(istringstream(to_utf8string(s)));
		}
		return task_from_result(imageMetadata);
	});
}

ImageMetadata TepisClient::parseMetadataResponseJSON(value json) {
	//printf("Processing JSON");
	json::object& root = json.as_object();
	json::object& imageMetadataObj = root[L"ImageMetadata"].as_object();
	json::object& pixelMetadataObj = imageMetadataObj[L"PixelMetadata"].as_object();
	json::array& pixelLevelMetadataArr = pixelMetadataObj[L"PixelLevelMetadata"].as_array();

	ImageMetadata imageMetadata;

	//TODO finish populating metadata
	//ScannerMetadata &scannerMetdata = imageMetadata.getScannerMetadata();
	//StainMetadata &stainMetadata = imageMetadata.getStainMetadata();
	//PrimaryAnatomicStructure &primaryAnatomicStructure = imageMetadata.getPrimaryAnatomicStructure();

	PixelMetadata &pixelMetadata = imageMetadata.getPixelMetadata();
	vector<PixelLevelMetadata> &levels = pixelMetadata.getLevels();

	for (auto& level : pixelLevelMetadataArr) {
		levels.push_back(PixelLevelMetadata(
			level[U("level")].as_integer(),
			to_utf8string(level[U("pixelSize")].as_string()),
			to_utf8string(level[U("physicalOrigin")].as_string()),
			to_utf8string(level[U("physicalSpacing")].as_string()),
			level[U("scanFactor")].as_double(),
			level[U("isNativeLevel")].as_bool(),
			level[U("isLossyCompressed")].as_bool(),
			level[U("lossyCompressionRatio")].as_integer(),
			to_utf8string(level[U("derivationDescription")].as_string()),
			to_utf8string(level[U("tileSize")].as_string())
			));
	}
	return imageMetadata;
}

ImageMetadata TepisClient::parseMetadataResponseXML(istringstream &iss) {
	//printf("Processing XML\n");
	ptree &pt = ptree();
	read_xml(iss, pt);
	ImageMetadata imageMetadata;

	//TODO finish populating metadata
	//ScannerMetadata &scannerMetdata = imageMetadata.getScannerMetadata();
	//StainMetadata &stainMetadata = imageMetadata.getStainMetadata();
	//PrimaryAnatomicStructure &primaryAnatomicStructure = imageMetadata.getPrimaryAnatomicStructure();

	PixelMetadata &pixelMetadata = imageMetadata.getPixelMetadata();
	vector<PixelLevelMetadata> &levels = pixelMetadata.getLevels();
	ptree &levelsTree = pt.get_child("ImageMetadata.PixelMetadata.Levels");
	for(const pair<string, ptree> &child : levelsTree) {
		ptree pixelLevelMetadata = child.second;
		PixelLevelMetadata level(
			pixelLevelMetadata.get<int>("Level"),
			pixelLevelMetadata.get<string>("PixelSize"),
			pixelLevelMetadata.get<string>("PhysicalOrigin"),
			pixelLevelMetadata.get<string>("PhysicalSpacing"),
			pixelLevelMetadata.get<double>("ScanFactor"),
			pixelLevelMetadata.get<bool>("IsNativeLevel"),
			pixelLevelMetadata.get<bool>("IsLossyCompressed"),
			pixelLevelMetadata.get<int>("LossyCompressionRatio"),
			pixelLevelMetadata.get<string>("DerivationDescription"),
			pixelLevelMetadata.get<string>("TileSize")
		);
		levels.push_back(level);
	}
	return imageMetadata;
}

void TepisClient::getAssociatedImage(const std::string &imageID)
{
}

task<http_response> TepisClient::getImagePixelData_HTTP(const std::string &imageID, float x, float y, float width, float height, int level, Unit unit) {
	http_request request(methods::GET);
	uri_builder uriBuilder = uri_builder(U("/ImageService"))
		.append_path(U("/image"))
		.append_path(to_string_t(imageID))
		.append_path(U("/pixeldata"))
		.append_query<float>(U("x"), x, true)
		.append_query<float>(U("y"), y, true)
		.append_query<float>(U("width"), width)
		.append_query<float>(U("height"), height)
		.append_query<int>(U("level"), level)
		.append_query(U("unit"), toString(unit));
	request.set_request_uri(uriBuilder.to_string());
	return httpClient.request(request);
}

vector<unsigned char> TepisClient::getImagePixelData(const std::string &imageID, float x, float y, float width, float height, int level, Unit unit) {
	return getImagePixelData_HTTP(imageID, x, y, width, height, level, unit).get().extract_vector().get();
}

task<vector<unsigned char>> TepisClient::getImagePixelData_Async(const std::string &imageID, float x, float y, float width, float height, int level, Unit unit)
{
	return getImagePixelData_HTTP(imageID, x, y, width, height, level, unit).then([](http_response response) {
		//printf("PixelData response status code:%u\n", response.status_code());
		//http_headers &headers = response.headers();
		//string_t &content_type = headers.content_type();
		return response.content_ready().then([] (http_response response) {
			//printf("Content Ready\n");
			return response.extract_vector().get();
		});
	});
}


void TepisClient::getTiledImagePixelData(const std::string &imageID)
{
}
