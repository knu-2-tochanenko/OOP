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
	std::ifstream i("backup.json");
	json j;
	i >> j;

	// write prettified JSON to another file
	std::ofstream o("pretty.json");
	o << std::setw(4) << j << std::endl;
	

	system("pause");
	return 0;
}