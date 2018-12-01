#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <vector>
#include <string>
#include <cstring>
#include "hpdf.h"
#include "json.hpp"

using namespace std;

using json = nlohmann::json;
//////////////////////////////////////////////////////
jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler(HPDF_STATUS   error_no,
	HPDF_STATUS   detail_no,
	void         *user_data) {
	printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
		(HPDF_UINT)detail_no);
	longjmp(env, 1);
}

const char *font_list[] = {
	"Courier",
	"Courier-Oblique",
	NULL
};
//////////////////////////////////////////////////////

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

	char* const to_string(string const str) const {
		char *res;
		res = new char[str.size()];
		strcpy(res, str.c_str());
		cout << res << endl;
		return res;
	}

	void printNotesText(string const & text, HPDF_Page page) const {
		// Max length - 57
		int strLen = text.size(), charStringSize = 0;
		string charString;
		for (int i = 0; i < strLen; i++) {
			if ((text[i] == '\n') || (charString.size() > 56)) {
				HPDF_Page_ShowText(page, to_string(charString));
				HPDF_Page_MoveTextPos(page, 0, -20);
				charString = "";
			}
			else
				charString += text[i];
		}
		if (charString.size() > 0) {
			HPDF_Page_ShowText(page, to_string(charString));
			HPDF_Page_MoveTextPos(page, 0, -20);
		}
	}

	// add &
	void writeNote(SingleNote* const sn, int shift, HPDF_Doc pdf, HPDF_Page page, HPDF_Font def_font) const {
		HPDF_Font font = HPDF_GetFont(pdf, font_list[0], NULL);

		/* print a sample text. */
		HPDF_Page_SetFontAndSize(page, font, 14);
		printNotesText(sn->text, page);
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

//		testVecNotes(this->notes, "NOTES");
//		testVecNotes(this->archive, "ARCHIVE");

		return true;
	}

	bool generate(string filepath = "D:/", string username = "", string password = "") {
		cout << filepath << endl;
		cout << username << endl;
		cout << password << endl;

		const char *notesText = "Notes";
		const char *archiveText = "Archived Notes";
		HPDF_Doc  pdf;
		string filename = "D:\\Vladislav\\Documents\\file.pdf";
		char fname[256];
		strcpy(fname, filename.c_str());
		HPDF_Page page;
		HPDF_Font def_font;
		HPDF_REAL tw;
		HPDF_REAL height;
		HPDF_REAL width;
		HPDF_UINT i;

		pdf = HPDF_New(error_handler, NULL);
		if (!pdf) {
			printf("error: cannot create PdfDoc object\n");
			return 1;
		}

		if (setjmp(env)) {
			HPDF_Free(pdf);
			return 1;
		}

		/* Add a new page object. */
		page = HPDF_AddPage(pdf);

		height = HPDF_Page_GetHeight(page);
		width = HPDF_Page_GetWidth(page);

		/* Print the title of the page (with positioning center). */
		def_font = HPDF_GetFont(pdf, "Helvetica", NULL);
		HPDF_Page_SetFontAndSize(page, def_font, 24);

		tw = HPDF_Page_TextWidth(page, notesText);
		HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page, (width - tw) / 2, height - 50, notesText);
		HPDF_Page_EndText(page);

		// Write notes
		HPDF_Page_BeginText(page);
		HPDF_Page_MoveTextPos(page, 60, height - 105);
		for (int i = 0; i < this->notes.size(); i++)
			writeNote(this->notes[i], -20, pdf, page, def_font);
		HPDF_Page_EndText(page);

		/* Print the title of the page (with positioning center). */
		def_font = HPDF_GetFont(pdf, "Helvetica", NULL);
		HPDF_Page_SetFontAndSize(page, def_font, 24);

		tw = HPDF_Page_TextWidth(page, archiveText);
		HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page, (width - tw) / 2, height - 100, archiveText);
		HPDF_Page_EndText(page);
		

		HPDF_SaveToFile(pdf, fname);

		/* clean up */
		HPDF_Free(pdf);

		return true;
	}
};

//	Uses FUP (filepath username password)
int main(int argumentsCount, char** arguments) {
	RJDP rjdp;
	rjdp.read("backup.json");
	
	if (argumentsCount == 2)
		rjdp.generate(arguments[1]);
	else if (argumentsCount == 4) {
		rjdp.generate(arguments[1], arguments[2], arguments[3]);
	}
	else return 1;

	system("pause");
	return 0;
}