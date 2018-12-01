#include <setjmp.h>
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <vector>
#include <string>
#include "hpdf.h"
#include "json.hpp"

using namespace std;

using json = nlohmann::json;

int main() {

	// read a JSON file
	std::ifstream jsonInput("backup.json");
	json jsonObject;
	jsonInput >> jsonObject;

	json notes = jsonObject["notes"];
	json archive = jsonObject["archive"];
	json maxID = jsonObject["max_id"];
	json tags = jsonObject["tags"];

	cout << notes << endl;
	cout << archive << endl;
	cout << maxID << endl;
	cout << tags << endl;

	system("pause");
	return 0;
}