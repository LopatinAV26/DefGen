#include "database.h"

database &database::Instance() {
	static database singleInstatnce;
	return singleInstatnce;
}

int database::SingleSelect(std::string dbName, std::string SELECT,
	std::string tbName, std::string WHERE, const int &colData) {

	rc = sqlite3_open_v2(dbName.c_str(), &db, SQLITE_OPEN_READONLY, NULL);
	if (rc != SQLITE_OK)
		wxLogFatalError(sqlite3_errmsg(db));

	std::string str = "SELECT " + SELECT + " FROM " + tbName + " WHERE " + WHERE + " = " + std::to_string(colData) + ";";
	const char *sql = str.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	int nColumns = sqlite3_column_count(stmt);

	int result = 0;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		for (int i = 0; i < nColumns; ++i) {
			result = sqlite3_column_int(stmt, i);
		}
	}
	if (rc != SQLITE_DONE)
		wxLogError(sqlite3_errmsg(db));

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	rc = sqlite3_close(db);
	if (rc != SQLITE_OK)
		wxLogFatalError(sqlite3_errmsg(db));

	return result;
}

std::string database::SingleSelect(std::string dbName, std::string SELECT,
	std::string tbName, std::string WHERE, const std::string colData) {

	rc = sqlite3_open_v2(dbName.c_str(), &db, SQLITE_OPEN_READONLY, NULL);
	if (rc != SQLITE_OK)
		wxLogFatalError(sqlite3_errmsg(db));

	std::string str = "SELECT " + SELECT + " FROM " + tbName + " WHERE " + WHERE + " = " + "'" + colData + "'" + ";";
	const char *sql = str.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	int nColumns = sqlite3_column_count(stmt);

	std::string result;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		for (int i = 0; i < nColumns; ++i) {
			result = (char *)(sqlite3_column_text(stmt, i));
		}
	}
	if (rc != SQLITE_DONE)
		wxLogError(sqlite3_errmsg(db));

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	rc = sqlite3_close(db);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	return result;
}

void database::SQLiteOpen(std::string dbName) {
	rc = sqlite3_open_v2(dbName.c_str(), &db, SQLITE_OPEN_READONLY, NULL);
	if (rc != SQLITE_OK)
		wxLogFatalError(sqlite3_errmsg(db));
}

bool database::SQLiteBoolSelect(wxString tableName, wxString colName, const wxInt32 rowID) {
	wxString str = "SELECT " + colName + " FROM " + tableName + " WHERE ID " + " = " + std::to_string(rowID) + ";";
	const char *sql = str.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	wxInt32 nColumns = sqlite3_column_count(stmt);

	bool result{ false };
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		for (std::size_t i = 0; i < nColumns; ++i) {
			result = sqlite3_column_int(stmt, i);
		}
	}
	if (rc != SQLITE_DONE)
		wxLogError(sqlite3_errmsg(db));

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	return result;
}

wxDouble database::SQLiteDoubleSelect(wxString tableName, wxString colName, std::string WHERE, const wxInt32 &WHEREData) {
	wxString str = "SELECT " + colName + " FROM " + tableName + " WHERE " + WHERE + " = " + std::to_string(WHEREData) + ";";
	const char *sql = str.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	wxInt32 nColumns = sqlite3_column_count(stmt);

	wxDouble result = 0.0;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		for (std::size_t i = 0; i < nColumns; ++i) {
			result = sqlite3_column_double(stmt, i);
		}
	}
	if (rc != SQLITE_DONE)
		wxLogError(sqlite3_errmsg(db));

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	return result;
}

wxInt32 database::SQLiteIntSelect(wxString tableName, wxString colName, const wxInt32 rowID) {
	wxString str = "SELECT " + colName + " FROM " + tableName + " WHERE ID " + " = " + std::to_string(rowID) + ";";
	const char *sql = str.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	wxInt32 nColumns = sqlite3_column_count(stmt);

	wxInt32 result = 0;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		for (std::size_t i = 0; i < nColumns; ++i) {
			result = sqlite3_column_int(stmt, i);
		}
	}
	if (rc != SQLITE_DONE)
		wxLogError(sqlite3_errmsg(db));

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	return result;
}

wxInt32 database::SQLiteIntSelect(wxString tableName, wxString colName, std::string WHERE, const wxInt32 &WHEREData) {
	wxString str = "SELECT " + colName + " FROM " + tableName + " WHERE " + WHERE + " = " + std::to_string(WHEREData) + ";";
	const char *sql = str.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	wxInt32 nColumns = sqlite3_column_count(stmt);

	wxInt32 result = 0;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		for (std::size_t i = 0; i < nColumns; ++i) {
			result = sqlite3_column_int(stmt, i);
		}
	}
	if (rc != SQLITE_DONE)
		wxLogError(sqlite3_errmsg(db));

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	return result;
}

wxString database::SQLiteStrSelect(wxString tableName, wxString colName, const wxInt32 rowID) {
	wxString str = "SELECT " + colName + " FROM " + tableName + " WHERE ID " + " = " + std::to_string(rowID) + ";";
	const char *sql = str.c_str();

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	wxInt32 nColumns = sqlite3_column_count(stmt);

	wxString result;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		for (std::size_t i = 0; i < nColumns; ++i) {
			result = wxString::FromUTF8((char *)(sqlite3_column_text(stmt, i)));
		}
	}
	if (rc != SQLITE_DONE)
		wxLogError(sqlite3_errmsg(db));

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));

	return result;
}

void database::SQLiteClose() {
	rc = sqlite3_close(db);
	if (rc != SQLITE_OK)
		wxLogError(sqlite3_errmsg(db));
}
