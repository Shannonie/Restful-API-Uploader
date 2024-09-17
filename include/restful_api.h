#pragma once
#ifndef RESTFUL_API_H
#define RESTFUL_API_H

#include <cpprest/json.h>
#include <cpprest/http_client.h>

#include <iostream>
#include <string>

#include "xditk_restful_api_export.h"

namespace ditalg
{
	class xditk_restful_api_export RestfulApi
	{
	public:
		RestfulApi(int mode, std::string address);
		~RestfulApi();

		bool SendPostResult(std::string proj_name, std::string sn_name, std::string obj_name, std::string& inferenc_result);

	private:
		web::http::http_request CreatePostRequest(std::wstring url);
		web::http::http_response SendPostRequest(web::http::http_request& request, web::json::value post_parameters);

		utility::string_t m_request_address;
		int m_detect_mode; //0:yolo; 1:segmentation; 2:efficientNet
		std::string m_inferenc_result;	
	};
}

#endif // RESTFULAPI_H"