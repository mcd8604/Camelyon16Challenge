#include "tepis_http_client.h"



tepis_http_client::tepis_http_client()
{
}


tepis_http_client::~tepis_http_client()
{
}

task<http_response> getImageMetadata(const string &imageID) {
	// Create a request to the tEPIS ImageService to get image metadata as JSON content
	uri_builder uriBuilder = uri_builder(U("/ImageService")).append_path(U("/image")).append_path(to_string_t(imageID)).append_path(U("/metadata"));
	http_request request(methods::GET);
	request.set_request_uri(uriBuilder.to_string());
	request.headers().add(header_names::accept, mime_types::application_json);

	// Invoke the request
	task<http_response> response = httpClient.request(request);
}