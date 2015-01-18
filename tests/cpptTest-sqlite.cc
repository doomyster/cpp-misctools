/*
    Copyright © 2013-2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cpptTest-sqlite.h"
#include <iostream>
#include <stdexcept>

CPPUNIT_TEST_SUITE_REGISTRATION( cpptTestSqlite );

// Dirty class to retrieve protected values of the cppmt::Sqlite::result_t class
class test_result_t: public cppmt::Sqlite::result_t
{
	private:
	test_result_t() {}

	public:
	const cppmt::Sqlite::result_t::rows_vector_t&                getRowsVector() const { return rows__; }
	const cppmt::Sqlite::result_t::rows_vector_t::const_iterator getCurrent() const    { return current__; }
};

void cpptTestSqlite::testSimpleRequests()
{
	cppmt::Sqlite s;
	std::string query;

	try {
		sqlite3_uint64 id;
		cppmt::Sqlite::result_t res;
		s.exec(query = "CREATE TABLE testing (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, data TEXT, misc TEXT)", cppmt::Sqlite::no_result);

		s.exec(query = "INSERT INTO testing (data) VALUES ('Aupr&egrave;s de mon arbre,')", cppmt::Sqlite::no_result);
		id = s.exec(query = "INSERT INTO testing (data) VALUES ('Je vivais heureux,')", cppmt::Sqlite::rowid);
		s.exec(query = "INSERT INTO testing (data, misc) VALUES ('J''aurais jamais d&ucirc; m''&eacute;loigner d'' mon arbre...', 'test_null')", cppmt::Sqlite::no_result);
		s.exec(query = "INSERT INTO testing (data) VALUES ('Aupr&egrave;s de mon arbre,')", cppmt::Sqlite::no_result);
		res = s.exec(query = "INSERT INTO testing (data) VALUES ('J''aurais jamais d&ucirc; le quitter des yeux...')");

		CPPUNIT_ASSERT(id == 2);
		CPPUNIT_ASSERT(res.numRows() == 0);
		try {
			res.numCells();
			CPPUNIT_FAIL("Call to res.numCells() should have thrown an 'out_of_range' exception.");
		}
		catch(const std::out_of_range& e) {
			// This is the good behaviour
		}

		res = s.exec("SELECT * FROM testing ORDER BY id DESC");
		CPPUNIT_ASSERT(res.numRows() == 5);
		CPPUNIT_ASSERT(res.numCells() == 3);
		{
			// Makes sure the iterator is valid (had a bug on this).
			test_result_t* tr = reinterpret_cast<test_result_t*>(&res);
			CPPUNIT_ASSERT(tr->getCurrent() == tr->getRowsVector().end());
		}

		// Unrolled version of the while(res.nextRow()) { ... } loop:
		CPPUNIT_ASSERT(res.nextRow() == true);
		CPPUNIT_ASSERT(res.getColumnName(0)    == "id"  );
		CPPUNIT_ASSERT(res.getColumnName(1)    == "data");
		CPPUNIT_ASSERT(res.getColumnName(2)    == "misc");

		CPPUNIT_ASSERT(res.isNull(0)           == false);
		CPPUNIT_ASSERT(res.isNull(1)           == false);
		CPPUNIT_ASSERT(res.isNull(2)           ==  true);

		{
			CPPUNIT_ASSERT(res.getValue(0)         == "5");
			CPPUNIT_ASSERT(res.getValue("id")      == "5");
			CPPUNIT_ASSERT(res.getValue<int>(0)    ==  5 );
			CPPUNIT_ASSERT(res.getValue<int>("id") ==  5 );

			CPPUNIT_ASSERT(res.getValue(1)         == "J'aurais jamais d&ucirc; le quitter des yeux...");
			CPPUNIT_ASSERT(res.getValue("data")    == "J'aurais jamais d&ucirc; le quitter des yeux...");

			CPPUNIT_ASSERT(res.getValue(2)         == "");
			CPPUNIT_ASSERT(res.getValue("misc")    == "");
		}

		CPPUNIT_ASSERT(res.nextRow() == true);
		CPPUNIT_ASSERT(res.getColumnName(0)    == "id"  );
		CPPUNIT_ASSERT(res.getColumnName(1)    == "data");
		CPPUNIT_ASSERT(res.getColumnName(2)    == "misc");

		CPPUNIT_ASSERT(res.isNull(0)           == false);
		CPPUNIT_ASSERT(res.isNull(1)           == false);
		CPPUNIT_ASSERT(res.isNull(2)           ==  true);

		{
			CPPUNIT_ASSERT(res.getValue(0)         == "4");
			CPPUNIT_ASSERT(res.getValue("id")      == "4");
			CPPUNIT_ASSERT(res.getValue<int>(0)    ==  4 );
			CPPUNIT_ASSERT(res.getValue<int>("id") ==  4 );

			CPPUNIT_ASSERT(res.getValue(1)         == "Aupr&egrave;s de mon arbre,");
			CPPUNIT_ASSERT(res.getValue("data")    == "Aupr&egrave;s de mon arbre,");

			CPPUNIT_ASSERT(res.getValue(2)         == "");
			CPPUNIT_ASSERT(res.getValue("misc")    == "");
		}

		CPPUNIT_ASSERT(res.nextRow() == true);
		CPPUNIT_ASSERT(res.getColumnName(0)    == "id"  );
		CPPUNIT_ASSERT(res.getColumnName(1)    == "data");
		CPPUNIT_ASSERT(res.getColumnName(2)    == "misc");

		CPPUNIT_ASSERT(res.isNull(0)           == false);
		CPPUNIT_ASSERT(res.isNull(1)           == false);
		CPPUNIT_ASSERT(res.isNull(2)           == false);

		{
			CPPUNIT_ASSERT(res.getValue(0)         == "3");
			CPPUNIT_ASSERT(res.getValue("id")      == "3");
			CPPUNIT_ASSERT(res.getValue<int>(0)    ==  3 );
			CPPUNIT_ASSERT(res.getValue<int>("id") ==  3 );

			CPPUNIT_ASSERT(res.getValue(1)         == "J'aurais jamais d&ucirc; m'&eacute;loigner d' mon arbre...");
			CPPUNIT_ASSERT(res.getValue("data")    == "J'aurais jamais d&ucirc; m'&eacute;loigner d' mon arbre...");

			CPPUNIT_ASSERT(res.getValue(2)         == "test_null");
			CPPUNIT_ASSERT(res.getValue("misc")    == "test_null");
		}

		CPPUNIT_ASSERT(res.nextRow() == true);
		CPPUNIT_ASSERT(res.getColumnName(0)    == "id"  );
		CPPUNIT_ASSERT(res.getColumnName(1)    == "data");
		CPPUNIT_ASSERT(res.getColumnName(2)    == "misc");

		CPPUNIT_ASSERT(res.isNull(0)           == false);
		CPPUNIT_ASSERT(res.isNull(1)           == false);
		CPPUNIT_ASSERT(res.isNull(2)           ==  true);

		{
			CPPUNIT_ASSERT(res.getValue(0)         == "2");
			CPPUNIT_ASSERT(res.getValue("id")      == "2");
			CPPUNIT_ASSERT(res.getValue<int>(0)    ==  2 );
			CPPUNIT_ASSERT(res.getValue<int>("id") ==  2 );

			CPPUNIT_ASSERT(res.getValue(1)         == "Je vivais heureux,");
			CPPUNIT_ASSERT(res.getValue("data")    == "Je vivais heureux,");

			CPPUNIT_ASSERT(res.getValue(2)         == "");
			CPPUNIT_ASSERT(res.getValue("misc")    == "");
		}

		CPPUNIT_ASSERT(res.nextRow() == true);
		CPPUNIT_ASSERT(res.getColumnName(0)    == "id"  );
		CPPUNIT_ASSERT(res.getColumnName(1)    == "data");
		CPPUNIT_ASSERT(res.getColumnName(2)    == "misc");

		CPPUNIT_ASSERT(res.isNull(0)           == false);
		CPPUNIT_ASSERT(res.isNull(1)           == false);
		CPPUNIT_ASSERT(res.isNull(2)           ==  true);

		{
			CPPUNIT_ASSERT(res.getValue(0)         == "1");
			CPPUNIT_ASSERT(res.getValue("id")      == "1");
			CPPUNIT_ASSERT(res.getValue<int>(0)    ==  1 );
			CPPUNIT_ASSERT(res.getValue<int>("id") ==  1 );

			CPPUNIT_ASSERT(res.getValue(1)         == "Aupr&egrave;s de mon arbre,");
			CPPUNIT_ASSERT(res.getValue("data")    == "Aupr&egrave;s de mon arbre,");

			CPPUNIT_ASSERT(res.getValue(2)         == "");
			CPPUNIT_ASSERT(res.getValue("misc")    == "");
		}

		CPPUNIT_ASSERT(res.nextRow() == false);

		s.exec("SELECT id AS nId, misc FROM testing WHERE id = 3", &res);
		CPPUNIT_ASSERT(res.numRows() == 1);
		const cppmt::Sqlite::row_t& row = res.getRow(0);

		CPPUNIT_ASSERT(res.numCells() == 2);
		CPPUNIT_ASSERT(row.numCells() == 2);

		CPPUNIT_ASSERT(row.getColumnName(0)     == "nId");
		CPPUNIT_ASSERT(row.getColumnName(1)     == "misc");
		CPPUNIT_ASSERT(row.getValue(1)          == "test_null");
		CPPUNIT_ASSERT(row.getValue("misc")     == "test_null");
		CPPUNIT_ASSERT(row.getValue(0)          == "3");
		CPPUNIT_ASSERT(row.getValue("nId")      == "3");
		CPPUNIT_ASSERT(row.getValue<int>(0)     ==  3 );
		CPPUNIT_ASSERT(row.getValue<int>("nId") ==  3 );

		try {
			const cppmt::Sqlite::row_t& r = res.getCurrentRow();
			CPPUNIT_FAIL("res.getCurrentRow() should have thrown an exception."); // Need to call for res.nextRow() before
		}
		catch(const std::out_of_range& e) {
			// Expected behaviour
		}

		// Test for copy constructor, should keep current__ to end
		{
			cppmt::Sqlite::result_t nres(res);
			test_result_t*  tr = reinterpret_cast<test_result_t*>(& res);
			test_result_t* ntr = reinterpret_cast<test_result_t*>(&nres);

			CPPUNIT_ASSERT( tr->getCurrent() ==  tr->getRowsVector().end());
			CPPUNIT_ASSERT(ntr->getCurrent() == ntr->getRowsVector().end());
		}

		try {
			const cppmt::Sqlite::row_t& r = res.getRow(1);
			CPPUNIT_FAIL("res.getRow(1) should have thrown an exception."); // Need to call for res.nextRow() before
		}
		catch(const std::out_of_range& e) {
			// Expected behaviour
		}

		// Test for copy constructor, should keep current__ to same item on the new vector
		{
			cppmt::Sqlite::result_t nres(res);
			test_result_t*  tr = reinterpret_cast<test_result_t*>(& res);
			test_result_t* ntr = reinterpret_cast<test_result_t*>(&nres);

			CPPUNIT_ASSERT( tr->getCurrent() != ntr->getCurrent());
			CPPUNIT_ASSERT(( tr->getCurrent() -  tr->getRowsVector().begin()) ==
			               (ntr->getCurrent() - ntr->getRowsVector().begin())     );

			// Test clear method
			nres.clear();
			CPPUNIT_ASSERT(nres.numRows() == 0);
			test_result_t* ztr = reinterpret_cast<test_result_t*>(& res);
			CPPUNIT_ASSERT(ztr->getCurrent() == ztr->getRowsVector().end());
		}


	}
	catch(const cppmt::sqlite_exception& e) {
		CPPUNIT_FAIL(query + ": " + e.what());
	}
}

