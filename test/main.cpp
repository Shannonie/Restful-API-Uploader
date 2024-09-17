#include <iostream>
#include <string>
#include "restful_api.h"

static ditalg::RestfulApi* g_restfulapi = NULL;

ditalg::RestfulApi* CreateRestfulApiInstance()
{
	int detect_mode = 0;
	int port = 9000;
	std::string host = "http://localhost";
	std::string address = host.append(":").append(std::to_string(port));

	return new ditalg::RestfulApi(detect_mode, address);
}

int main(int argc, char *args[])
{
	std::string inferenc_result = 
		"{'wo': 'WO#0001', 'pn': 'PN#0001', 'ln': 'LN#0001', 'workstation': 'WORKSTATION#0001', 'workstation_description': '<<description...>>', 'ProjectName': 'demo', 'sn': 'ASUS_0002', 'log': [{'ObjectName': 'Bracket', 'MainItems': [{'MainTestItemName': 'Object_Detection', 'RAW': './1-51.png', 'DEBUG_FILE': './1-51.png', 'SPEC_FILE': 'spec.ini', 'DATETIME': '20210426 131737', 'RESULT': 'FAIL', 'OTHERS': '', 'ERROR_CODE': 0, 'SubItems': [{'SubTestItemName': 'Defect_Class_ID', 'VALUE': 0, 'USL': 99999, 'LSL': 0, 'RESULT': 'PASS'}, {'SubTestItemName': 'Defect_Counter_ID', 'VALUE': 0, 'USL': 99999, 'LSL': 0, 'RESULT': 'PASS'}, {'SubTestItemName': 'Defect_Seg_1_PosX', 'VALUE': -200, 'USL': 99999, 'LSL': 0, 'RESULT': 'PASS'}, {'SubTestItemName': 'Defect_Seg_1_PosY', 'VALUE': 11, 'USL': 99999, 'LSL': 0, 'RESULT': 'PASS'}, {'SubTestItemName': 'Defect_Rect_Width', 'VALUE': 17, 'USL': 99999, 'LSL': 0, 'RESULT': 'PASS'}, {'SubTestItemName': 'Defect_Rect_Height', 'VALUE': 2, 'USL': 99999, 'LSL': 0, 'RESULT': 'PASS'}]}]}]}";
	g_restfulapi = CreateRestfulApiInstance();
	
	try
	{
		if (g_restfulapi == NULL)
		{
			throw "[ERROR] Failed to init.RestfulApi";
		}
		
		if (!g_restfulapi->SendPostResult("demo", "ASUS_0002", "Bracket", inferenc_result))
		{
			throw "[ERROR] Failed to send result";
		}
	}
	catch (const char* message)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}