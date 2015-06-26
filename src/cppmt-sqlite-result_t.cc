/*
    Copyright © 2014 Nicolas Normand <nnormand@justcodeit.org>. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Nicolas Normand "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cppmt-sqlite.h"
#include <stdexcept> 

namespace cppmt
{

Sqlite::result_t::result_t(): current__(rows__.end())
{
}

Sqlite::result_t::result_t(const Sqlite::result_t& r)
{
	*this = r;
}

Sqlite::result_t& Sqlite::result_t::operator= (const Sqlite::result_t& r)
{
	rows__ = r.rows__;
	current__ = rows__.begin() + (r.current__ - r.rows__.begin());
	return *this;
}

void Sqlite::result_t::clear()
{
	rows__.clear();
	current__ = rows__.end();
}

void Sqlite::result_t::add(int argc, char** argv, char** name)
{
	rows__.push_back(row_t(argc, argv, name));
}

const string& Sqlite::result_t::getValue(int idx) const
{
	return getCurrentRow().getValue(idx);
}

const string& Sqlite::result_t::getValue(const string& col) const
{
	return getCurrentRow().getValue(col);
}

bool Sqlite::result_t::isNull(int idx) const
{
	return getCurrentRow().isNull(idx);
}

bool Sqlite::result_t::isNull(const string& col) const
{
	return getCurrentRow().isNull(col);
}

const string& Sqlite::result_t::getColumnName(int idx) const
{
	return getCurrentRow().getColumnName(idx);
}

const Sqlite::row_t& Sqlite::result_t::getCurrentRow() const
{
	if (current__ == rows__.end()) {
		throw std::out_of_range("cppmt::Sqlite::result_t::getCurrentRow()");
	}
	return *current__;
}

const Sqlite::row_t& Sqlite::result_t::getRow(int idx) const
{
	return rows__.at(idx);
}

Sqlite::row_t::size_type Sqlite::result_t::numCells() const
{
	return rows__.at(0).numCells();
}

bool Sqlite::result_t::nextRow() const
{
	if (current__ == rows__.end()) {
		current__ = rows__.begin();
	}
	else {
		++current__;
	}

	return current__ != rows__.end();
}

Sqlite::result_t::size_type Sqlite::result_t::numRows() const
{
	return rows__.size();
}

} // end of: namespace cppmt

