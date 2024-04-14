#pragma once
#include <wx/log.h>
#include <wx/string.h>
#include "SQLite3/sqlite3.h"

class database {
public:
	static database &Instance();

	///Include: sqlite3_open_v2, sqlite3_prepare_v2, sqlite3_step, sqlite3_finalize, sqlite3_close
	wxInt32 SingleSelect(std::string dbName, std::string SELECT,
		std::string tbName, std::string WHERE, const int &colData);

	///Include: sqlite3_open_v2, sqlite3_prepare_v2, sqlite3_step, sqlite3_finalize, sqlite3_close
	std::string SingleSelect(std::string dbName, std::string SELECT,
		std::string tbName, std::string WHERE, const std::string colData);

	void SQLiteOpen(std::string dbName);
	void SQLiteClose();

	bool SQLiteBoolSelect(wxString tableName, wxString colName, const wxInt32 rowID);
	wxDouble SQLiteDoubleSelect(wxString tableName, wxString colName, std::string WHERE, const wxInt32 &WHEREData);
	wxInt32 SQLiteIntSelect(wxString tableName, wxString colName, const wxInt32 rowID);
	wxInt32 SQLiteIntSelect(wxString tableName, wxString colName, std::string WHERE, const wxInt32 &WHEREData);
	wxString SQLiteStrSelect(wxString tableName, wxString colName, const wxInt32 rowID);

private:
	database() {};
	database(const database &) {};
	const database &operator=(const database &) {};

	sqlite3 *db = nullptr;
	sqlite3_stmt *stmt = nullptr;
	int rc = 0;
};