#include "json_builder.h"

ditalg::JsonBuilder::JsonBuilder(const int& mode)
	:m_mainTestItem_name("")
{
	m_detect_mode = mode;
	switch (m_detect_mode)
	{
	case 1: //segmentation
		m_mainTestItem_name = "Segmentation";
		m_subItems_name = { "Defect_Class_ID","Defect_Counter_ID", "Defect_Seg_1_PosX","Defect_Seg_1_PosY",
			"Defect_PixelCount","Defect_Probability"};
		break;
	case 2: //effcientNet
		m_mainTestItem_name = "Classification";
		m_subItems_name = { "Label_Map_ID","Accuracy" };
		break;
	default: //YOLO
		m_mainTestItem_name = "Object_Detection";
		m_subItems_name = { "Defect_Class_ID","Defect_Counter_ID", "Defect_Seg_1_PosX","Defect_Seg_1_PosY",
			"Defect_Rect_Width","Defect_Rect_Height" };
		break;
	}
	
	m_log_data = web::json::value::array();
	m_mainItem_data = web::json::value::array();
}

ditalg::JsonBuilder::~JsonBuilder()
{
}

bool ditalg::JsonBuilder::ParseFromResult(std::string & result)
{
	int defect_num = 3;
	std::vector<std::string> values;

	if (m_detect_mode == 2)
	{
		values = { "1","0.59" };
		m_subItems_value.push_back(values);
	}
	else
	{
		for (int idx = 0; idx < defect_num; idx++)
		{
			std::string id = std::to_string(idx);

			if (m_detect_mode == 0)
			{
				values = { "0", id, "-200", "11", "17", "2" };
			}
			else
			{
				values = { "1", id, "-700", "-101", "60", "0.49" };
			}
			m_subItems_value.push_back(values);
			values.clear();
		}
	}

	return true;
}

bool ditalg::JsonBuilder::BuildJsonData(const std::string& obj_name, const std::string& proj_name, const std::string& sn, std::string& result)
{
	m_headers = { "wo","pn","ln","workstation","workstation_description","ProjectName","sn" };
	m_headers_value = { "WO#0001","PN#0001","LN#0001","WORKSTATION#0001","<<description...>>" };
	m_headers_value.push_back(proj_name);
	m_headers_value.push_back(sn);

	m_mainItems = { "MainTestItemName","RAW","DEBUG_FILE","SPEC_FILE","DATETIME","RESULT","OTHERS","ERROR_CODE","SubItems" };
	m_mainItems_value = { m_mainTestItem_name,"./xxx.png","./xxx.png","spec.ini","","FAIL","", };

	if (!ParseFromResult(result))
	{
		std::cout << "[ERROR] parse result fail" << std::endl;
		return false;
	}
	else
	{
		tm currnet_tm = GetCurrentTime();
		int hr, min = currnet_tm.tm_min, sec;

		std::stringstream month, day;
		month << std::setw(2) << std::setfill('0') << (1 + currnet_tm.tm_mon);
		day << std::setw(2) << std::setfill('0') << (currnet_tm.tm_mday);
		
		std::string date = std::to_string(1900 + currnet_tm.tm_year) + month.str() + day.str() + " ";

		// subItems info
		for (int count = 0; count < m_subItems_value.size(); count++)
		{
			std::stringstream hour, minute, second;
			sec = currnet_tm.tm_sec + count;
			if (sec > 60)
			{
				sec -= 60;
				min = currnet_tm.tm_min + 1;
				if (min > 60)
				{
					min -= 60;
					hr = currnet_tm.tm_hour + 1;
				}
				else
				{
					hr = currnet_tm.tm_hour;
				}
			}
			else
			{
				min = currnet_tm.tm_min;
				hr = currnet_tm.tm_hour;
			}

			hour << std::setw(2) << std::setfill('0') << (hr);
			minute << std::setw(2) << std::setfill('0') << (min);
			second << std::setw(2) << std::setfill('0') << (sec);
			
			m_mainItems_value[4] = date + hour.str() + minute.str() + second.str();

			// mainItems info
			web::json::value main;

			for (int idx = 0; idx < m_mainItems_value.size(); idx++)
			{
				main[ConvertStr(m_mainItems[idx])] = web::json::value::string(ConvertStr(m_mainItems_value[idx]));
			}

			main[ConvertStr(m_mainItems[7])] = web::json::value::number(0);
			main[ConvertStr(m_mainItems[8])] = web::json::value::array(CreateSubItem(count));

			m_mainItem_data[count] = main;
		}
		
		// log info
		web::json::value logs;
		std::string object_name = obj_name;
		logs[L"ObjectName"] = web::json::value::string(ConvertStr(object_name));
		logs[L"MainItems"] = m_mainItem_data;
		m_log_data[0] = logs;
		
		// headers info
		for (int index = 0; index < m_headers.size(); index++)
		{
			m_json_data[ConvertStr(m_headers[index])] = web::json::value::string(ConvertStr(m_headers_value[index]));
		}
		m_json_data[L"log"] = m_log_data;
		
		///std::wcout << json_data << std::endl;
	}

	return true;
}

web::json::value ditalg::JsonBuilder::GetJsonParameters()
{
	return m_json_data;
}

std::vector<web::json::value> ditalg::JsonBuilder::CreateSubItem(int index)
{
	std::vector<web::json::value> subItem;
	std::vector<std::string> values = m_subItems_value[index];
	
	for (int idx = 0; idx < m_subItems_name.size(); idx++)
	{
		web::json::value item;
		item[L"SubTestItemName"] = web::json::value::string(ConvertStr(m_subItems_name[idx]));
		
		std::string::size_type sz;
		if (m_detect_mode != 0 && idx == (m_subItems_name.size() - 1))
		{
			double num = std::stod(values[idx], &sz);
			item[L"VALUE"] = web::json::value::number(num);
		}
		else
		{
			item[L"VALUE"] = web::json::value::number(index);
		}
		
		item[L"USL"] = web::json::value::number(9999);
		item[L"LSL"] = web::json::value::number(0);
		item[L"RESULT"] = web::json::value::string(L"PASS");
		
		subItem.push_back(item);
	}

	return subItem;
}

std::wstring ditalg::JsonBuilder::ConvertStr(std::string str)
{
	return std::wstring(str.begin(), str.end());
}

tm ditalg::JsonBuilder::GetCurrentTime()
{
	struct tm ltm;
	time_t now = clock();
	time(&now);
	localtime_s(&ltm, &now);

	return ltm;
}