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
jmp_buf env;

//	This is needed for PDF
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

//	Used fonts
const char *font_list[] = {
	"Courier",
	"Courier-Oblique",
	NULL
};

//	Structure to store single note
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
		return res;
	}

	void printTextToPDF(string const & text, HPDF_Doc& pdf, HPDF_Page& page,
		int const shift, int const maxLength, HPDF_Font font, int fontSize) const {
		int strLen = text.size(), charStringSize = 0;
		string charString;
		for (int i = 0; i < strLen; i++) {
			if ((text[i] == '\n') || (charString.size() >= maxLength)) {
				HPDF_Page_ShowText(page, to_string(charString));
				page = ifNewPage(pdf, page, (-shift) + 1, font, fontSize);
				HPDF_Page_MoveTextPos(page, 0, shift);
				charString = "";
			}
			else
				charString += text[i];
		}
		if (charString.size() > 0) {
			HPDF_Page_ShowText(page, to_string(charString));
			page = ifNewPage(pdf, page, (-shift) + 1, font, fontSize);
			HPDF_Page_MoveTextPos(page, 0, shift);
		}
	}

	HPDF_Page ifNewPage(HPDF_Doc& pdf, HPDF_Page& page, int height, HPDF_Font font, int fontSize) const {
		HPDF_Point hp = HPDF_Page_GetCurrentTextPos(page);
		HPDF_Page newPage;
		if (hp.y < height * 3) {
			newPage = HPDF_AddPage(pdf);
			HPDF_Page_SetFontAndSize(newPage, font, fontSize);
			HPDF_Page_EndText(page);
			HPDF_Page_BeginText(newPage);

			HPDF_REAL pageHeight;
			HPDF_REAL pageWidth;

			HPDF_Point hp1 = HPDF_Page_GetCurrentTextPos(newPage);

			pageHeight = HPDF_Page_GetHeight(newPage);
			pageWidth = HPDF_Page_GetWidth(newPage);

			HPDF_Page_MoveTextPos(newPage, 60, pageHeight - height);
			return newPage;
		}
		return page;
	}

	void writeNote(SingleNote* const sn, HPDF_Doc& pdf, HPDF_Page& page, HPDF_Font& def_font) const {
		HPDF_Font regularFont = HPDF_GetFont(pdf, font_list[0], NULL);
		HPDF_Font italicFont = HPDF_GetFont(pdf, font_list[1], NULL);

		/*	PRINT NOTE TEXT  */
		HPDF_Page_SetFontAndSize(page, regularFont, 14);
		printTextToPDF(sn->text, pdf, page, -15, 57, regularFont, 14);

		HPDF_Page_SetFontAndSize(page, italicFont, 10);
		/*	PRINT NOTE TAGS  */
		string tags = "Tags : ";
		for (int i = 0; i < sn->tags.size() - 1; i++)
			tags += sn->tags[i] + "; ";
		tags += sn->tags[sn->tags.size() - 1];
		printTextToPDF(tags, pdf, page, -11, 80, italicFont, 10);

		/*	PRINT NOTE CREATOIN TIME & DATE  */
		string creationTime = "Creation time : " + sn->creationTime + " - " + sn->creationDate;
		printTextToPDF(creationTime, pdf, page, -11, 80, italicFont, 10);
		/*	PRINT NOTE EDITED TIME & DATE  */
		string editedTime = "Last edited time : " + sn->editedTime + " - " + sn->editedDate;
		printTextToPDF(editedTime, pdf, page, -11, 80, italicFont, 10);

		/*	DRAW A LINE  */
		// Can't draw a line with HPDF_Page_LineTo() because it looses focus. Currently is a bug
		printTextToPDF("________________________________________________________________________________",
			pdf, page, -11, 80, italicFont, 10);

		HPDF_Page_MoveTextPos(page, 0, -20);
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

	bool generate(string filepath = "D:\\file.pdf", string ownerPassword = "", string userPassword = "") {
		const char *notesText = "Notes";
		const char *archiveText = "Archived Notes";
		const char *tagsText = "Tags";
		HPDF_Doc  pdf;
		char fname[256];
		strcpy(fname, filepath.c_str());
		HPDF_Page page;
		HPDF_Font def_font;
		HPDF_REAL tw;
		HPDF_REAL height;
		HPDF_REAL width;

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

		/*	NOTES */
		def_font = HPDF_GetFont(pdf, "Helvetica", NULL);
		HPDF_Page_SetFontAndSize(page, def_font, 24);

		tw = HPDF_Page_TextWidth(page, notesText);
		HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page, (width - tw) / 2, height - 50, notesText);

		//	Write notes
		HPDF_Page_MoveTextPos(page, -((width - tw) / 2) + 60, -20);
		for (int i = 0; i < this->notes.size(); i++)
			writeNote(this->notes[i], pdf, page, def_font);

		/*	ARCHIVED NOTES */
		def_font = HPDF_GetFont(pdf, "Helvetica", NULL);
		HPDF_Page_SetFontAndSize(page, def_font, 24);

		tw = HPDF_Page_TextWidth(page, archiveText);
		HPDF_Page_MoveTextPos(page, 60, -30);
		HPDF_Page_TextOut(page, (width - tw) / 2, HPDF_Page_GetCurrentTextPos(page).y, archiveText);

		//	Write archived notes
		HPDF_Page_MoveTextPos(page, -((width - tw) / 2) + 60, -20);
		for (int i = 0; i < this->archive.size(); i++)
			writeNote(this->archive[i], pdf, page, def_font);

		/*	TAGS */
		def_font = HPDF_GetFont(pdf, "Helvetica", NULL);
		HPDF_Page_SetFontAndSize(page, def_font, 24);

		tw = HPDF_Page_TextWidth(page, tagsText);
		HPDF_Page_MoveTextPos(page, 60, -30);
		HPDF_Page_TextOut(page, (width - tw) / 2, HPDF_Page_GetCurrentTextPos(page).y, tagsText);

		//	Write tags
		HPDF_Page_MoveTextPos(page, -((width - tw) / 2) + 60, -20);
		HPDF_Font regularFont = HPDF_GetFont(pdf, font_list[0], NULL);
		HPDF_Font italicFont = HPDF_GetFont(pdf, font_list[1], NULL);
		HPDF_Page_SetFontAndSize(page, regularFont, 14);
		for (int i = 0; i < this->tags.size(); i++)
			printTextToPDF(this->tags[i], pdf, page, -15, 57, regularFont, 14);
		HPDF_Page_EndText(page);

		// Set passwords
		if (userPassword != "" && ownerPassword != "")
			HPDF_SetPassword(pdf, to_string(ownerPassword), to_string(userPassword));

		HPDF_SaveToFile(pdf, fname);

		/*	CLEAN  */
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