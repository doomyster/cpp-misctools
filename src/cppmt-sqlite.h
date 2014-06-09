/*
    Copyright © 2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CPP_MISCTOOLS_SQLITE3_H_
#define CPP_MISCTOOLS_SQLITE3_H_

#include <sqlite3.h>
#include "cppmt-exceptions.h"
#include "cppmt-string.h"
#include <map>
#include <vector>

namespace cppmt
{

class sqlite_exception: public exception
{
	public:
	class close_t {};
	static const close_t close_db;

	sqlite_exception(sqlite3* db,      const string& msg):                   exception((msg + ": ") + sqlite3_errmsg(db)) {}
	sqlite_exception(const string& fn, const string& msg):                   exception( fn  + ": "  + msg               ) {}
	sqlite_exception(sqlite3* db,      const string& msg, const close_t& c): exception((msg + ": ") + sqlite3_errmsg(db)) { sqlite3_close(db); }
	~sqlite_exception() throw() {}

	virtual const char* type() const throw() { return "sqlite_exception"; }
};

class Sqlite
{
	public:
	class no_result_t {};
	class rowid_t {};
	static const no_result_t no_result;
	static const rowid_t rowid;

	public:
	class cell_t
	{
		public:
		std::string value;
		std::string columnName;
		bool isNull;

		cell_t();
		cell_t(const char* val, const string& col);
	};

	class result_t;
	class row_t
	{
		friend class Sqlite;
		friend class result_t;
		public:
		typedef std::vector<cell_t> cells_vector_t;
		typedef cells_vector_t::const_iterator const_iterator;
		typedef cells_vector_t::size_type size_type;

		protected:
		std::vector<cell_t> cells__;
		std::map<string, int> cellsByName__;

		row_t();
		row_t(int argc, char** argv, char** name);

		public:
		size_type numCells() const;

		const string& getValue     (      int     idx) const;
		const string& getValue     (const string& col) const;
		bool          isNull       (      int     idx) const;
		bool          isNull       (const string& col) const;
		const string& getColumnName(      int     idx) const;

		template <typename T> T getValue(      int     idx) const;
		template <typename T> T getValue(const string& col) const;


		const cell_t& getCell(      int     idx) const;
		const cell_t& getCell(const string& col) const;

		const_iterator begin() const;
		const_iterator end  () const;
	};

	class result_t
	{
		friend class Sqlite;

		public:
		typedef std::vector<row_t> rows_vector_t;
		typedef rows_vector_t::const_iterator const_iterator;
		typedef rows_vector_t::size_type size_type;

		protected:
		rows_vector_t rows__;
		mutable rows_vector_t::const_iterator current__;

		result_t();

		void add(int argc, char** argv, char** name);

		public:
		const string& getValue     (int idx)           const;
		const string& getValue     (const string& col) const;
		bool          isNull       (int idx)           const;
		bool          isNull       (const string& col) const;
		const string& getColumnName(int idx)           const;

		template <typename T> T getValue(      int     idx) const;
		template <typename T> T getValue(const string& col) const;

		const row_t& getCurrentRow() const;
		const row_t& getRow(int idx) const;
		row_t::size_type numCells() const;

		bool nextRow() const; // Returns false on invalid entry (after the last one).
		size_type numRows() const;
	};

	protected:
	sqlite3* db__;

	static int cb_function(void* data, int argc, char** argv, char** colName);

	public:
	typedef int (*sqlite_cb)(void*, int, char**, char**); // data, argc, argv, column_name
	typedef std::vector<result_t> rows_t;

	Sqlite(const string& file = ":memory:");
	virtual ~Sqlite();

	void          exec(const string& query, const no_result_t& nores);
	void          exec(const string& query, sqlite_cb cb, void* data);
	sqlite3_int64 exec(const string& query, const rowid_t& rid);
	result_t      exec(const string& query);

	void saveInto(Sqlite& dest);
};

} // End of: namespace cppmt

#include "cppmt-sqlite.hxx"

#endif

