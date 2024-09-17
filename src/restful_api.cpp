#include "restful_api.h"
#include "json_builder.h"

namespace ditalg
{
	RestfulApi::RestfulApi(int mode, std::string address)
	{
		//init. params.
		std::cout << "[RestfulApi] request URL: " + address << std::endl;
		std::wstring ws_address(address.begin(), address.end());
		m_request_address = ws_address;
		m_detect_mode = mode;
	}

	RestfulApi::~RestfulApi()
	{
	}

	//called function for caller: send inferenced result
	bool RestfulApi::SendPostResult(std::string proj_name, std::string sn_name,
		std::string obj_name, std::string& inferenc_result)
	{  
		JsonBuilder json_builder(m_detect_mode);

		bool succeed_build_data = json_builder.BuildJsonData(obj_name, proj_name, sn_name, inferenc_result);
		if (!succeed_build_data)
		{
			std::cout << "[ERROR] bould json form fail" << std::endl;
			return false;
		}
		else
		{
			web::http::http_request request = CreatePostRequest(U("/api/SPC1.5/SendResults"));

			web::json::value json_object = json_builder.GetJsonParameters();

			web::http::http_response response = SendPostRequest(request, json_object);
			if (response.status_code() != web::http::status_codes::OK)
			{
				std::cout << "[ERROR] received response status code:" << response.status_code() << std::endl;
				return false;
			}

			///auto respon_body = response.extract_string();
			///std::wcout << L"- Response body: " << respon_body.get().c_str() << std::endl;
		}

		return true;
	}
	
	// create post request 
	web::http::http_request RestfulApi::CreatePostRequest(std::wstring url)
	{
		web::uri_builder builder(url);
		utility::string_t content_type = U("Content-Type");
		utility::string_t content_type_value = U("application/json");

		web::http::http_request request(web::http::methods::POST);
		request.set_request_uri(builder.to_uri());
		request.headers().add(content_type, content_type_value);

		return request;
	}

	// post to database implement
	web::http::http_response RestfulApi::SendPostRequest(web::http::http_request& request,
		web::json::value post_parameters)
	{
		//init. http_client
		web::http::client::http_client_config http_client_cfg = web::http::client::http_client_config();
		http_client_cfg.set_timeout(utility::seconds(3));
		web::http::client::http_client* m_http_client_impl = new web::http::client::http_client(m_request_address, http_client_cfg);

		web::json::value body = web::json::value::array();
		body[0] = post_parameters;
		request.set_body(body);
		
		std::cout << "[RestfulApi] sending request......" << std::endl;
		clock_t begin = clock();
		web::http::http_response response = m_http_client_impl->request(request).get();
		clock_t end = clock();
		double elapsed_secs = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
		std::cout << "[RestfulApi] get response status code: " + std::to_string(response.status_code()) << std::endl;
		std::cout << "[RestfulApi] time elapsed: " + std::to_string(elapsed_secs*1000) + " ms." << std::endl;
		
		return response;
	}
}