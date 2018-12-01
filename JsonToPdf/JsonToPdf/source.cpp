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

struct SingleNote {
	string text;
	string creationTime;
	string creationDate;
	string editedTime;
	string editedDate;
	vector<string> tags;
	
	SingleNote(string text, string creationTime, string creationDate,
		string editedTime, string editedDate, vector<string> tags) {
		this->text = text;
		this->creationTime = creationTime;
		this->creationDate = creationDate;
		this->editedTime = editedTime;
		this->editedDate = editedDate;
		this->tags = tags;
	}

};

class RJDP { // Read JSON, generate PDF
private:
	vector<SingleNote*> notes;
	vector<SingleNote*> archive;
	vector<string> tags;

	void testVecNotes(vector<SingleNote*> const & vec, string const name) const {
		cout << name << endl;
		for (auto i : vec) {
			cout << "text\t\t:\t" << i->text << endl;
			cout << "creationTime\t:\t" << i->creationTime << endl;
			cout << "creationDate\t:\t" << i->creationDate << endl;
			cout << "editedTime\t:\t" << i->editedTime << endl;
			cout << "editedDate\t:\t" << i->editedDate << endl << endl;
		}
	}

	void jsonToVecNotes(vector<SingleNote*> & vec, json jsonObject) {
		vector<json> notesList;
		notesList = jsonObject.get<vector<json>>();
		for (int i = 0; i < notesList.size(); i++) {
			SingleNote* sn = new SingleNote(
				notesList[i]["note_text"].get<string>(),
				notesList[i]["creation_time"].get<string>(),
				notesList[i]["creation_date"].get<string>(),
				notesList[i]["edited_time"].get<string>(),
				notesList[i]["edited_date"].get<string>(),
				notesList[i]["tags"].get<vector<string>>());
			vec.push_back(sn);
		}
	}

public:
	bool read(string const filename) {
		std::ifstream jsonInput(filename);
		json jsonObject;
		jsonInput >> jsonObject;

		// Get values from JSON file
		json notesJson = jsonObject["notes"];
		json archiveJson = jsonObject["archive"];
		json tagsJson = jsonObject["tags"];

		// Convert JSON objects to standard
		jsonToVecNotes(this->notes, notesJson);
		jsonToVecNotes(this->archive, archiveJson);
		this->tags = tagsJson.get<vector<string>>();

		testVecNotes(this->notes, "NOTES");
		testVecNotes(this->archive, "ARCHIVE");

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