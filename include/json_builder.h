#ifndef JSON_BUILDER_H
#define JSON_BUILDER_H

#include <cpprest/json.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

namespace ditalg
{
	class JsonBuilder
	{
	public:
		JsonBuilder(const int& mode);			
		~JsonBuilder();

		bool BuildJsonData(const std::string& obj_name, const std::string& proj_name, const std::string& sn, std::string& result);
		web::json::value GetJsonParameters();

	private:
		std::wstring ConvertStr(std::string str);
		tm GetCurrentTime();
		bool ParseFromResult(std::string& result);
		std::vector<web::json::value> CreateSubItem(int index);

		std::vector<std::string> m_headers;
		std::vector<std::string> m_headers_value;
		std::vector<std::string> m_mainItems;
		std::vector<std::string> m_mainItems_value;
		std::vector<std::string> m_subItems_name;
		std::vector<std::vector<std::string>> m_subItems_value;

		int m_detect_mode;
		std::string m_mainTestItem_name;

		web::json::value m_json_data;
		web::json::value m_log_data;
		web::json::value m_mainItem_data;
	};
}

#endif // !JSON_BUILDER_H
