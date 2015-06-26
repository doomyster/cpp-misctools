#include "cppmt-sqlite.h"
#include <stdexcept> 

namespace cppmt
{

const sqlite_exception::close_t sqlite_exception::close_db = sqlite_exception::close_t();
const Sqlite::no_result_t Sqlite::no_result = Sqlite::no_result_t();
const Sqlite::    rowid_t Sqlite::rowid = Sqlite::rowid_t();


Sqlite::Sqlite(const string& file)
{
	int rc = sqlite3_open(file.c_str(), &db__);
	if (rc) {
		throw sqlite_exception(db__, "open", sqlite_exception::close_db);
	}
}

Sqlite::~Sqlite()
{
	sqlite3_close(db__);
}

void Sqlite::exec(const string& query, const Sqlite::no_result_t& nores)
{
	exec(query, NULL, NULL);
}

void Sqlite::exec(const string& query, Sqlite::sqlite_cb cb, void* data)
{
	char* zError = 0;
	int rc = sqlite3_exec(db__, query.c_str(), cb, data, &zError);
	if (rc != SQLITE_OK) {
		string err = zError;
		sqlite3_free(zError);
		throw sqlite_exception("exec", err);
	}
}


int Sqlite::cb_function(void* data, int argc, char** argv, char** colName)
{
	Sqlite::result_t* r = reinterpret_cast< Sqlite::result_t* >(data);
	r->add(argc, argv, colName);
	r->current__ = r->rows__.end();
	return 0;
}

Sqlite::result_t Sqlite::exec(const string& query)
{
	Sqlite::result_t res;
	exec(query, Sqlite::cb_function, reinterpret_cast<void*>(&res));
	return res;
}

void Sqlite::exec(const string& query, Sqlite::result_t* res)
{
	res->clear();
	exec(query, Sqlite::cb_function, reinterpret_cast<void*>(res));
}

sqlite3_int64 Sqlite::exec(const string& query, const Sqlite::rowid_t& rid)
{
	exec(query, no_result);
	return sqlite3_last_insert_rowid(db__);
}

void Sqlite::saveInto(Sqlite& dest)
{
	sqlite3_backup* bc = sqlite3_backup_init(dest.db__, "main", db__, "main");
	if (bc) {
		sqlite3_backup_step(bc, -1);
		sqlite3_backup_finish(bc);
	}
	else {
		sqlite_exception(dest.db__, "backup_init");
	}
}

} // end of: namespace cppmt

