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

class RJDP { // Read JSON, generate PDF
private:
	vector<json> notes;
	vector<json> archive;
	vector<string> tags;
public:
	bool read(string const filename) {
		std::ifstream jsonInput(filename);
		json jsonObject;
		jsonInput >> jsonObject;

		json notesJson = jsonObject["notes"];
		json archiveJson = jsonObject["archive"];
		json tagsJson = jsonObject["tags"];

		this->notes = notesJson.get<vector<json>>();
		this->archive = archiveJson.get<vector<json>>();
		this->tags = tagsJson.get<vector<string>>();
		return true;
	}

	bool generate() const {
		return true;
	}
};

int main() {
	RJDP rjdp;
	rjdp.read("backup.json");
	

	system("pause");
	return 0;
}