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

Sqlite::row_t::row_t()
{
}

Sqlite::row_t::row_t(int argc, char** argv, char** name)
{
	for (int i=0; i<argc; ++i) {
		cells__.push_back(cell_t(argv[i], name[i]));
		cellsByName__[name[i]] = i;
	}
}

Sqlite::row_t::size_type Sqlite::row_t::numCells() const
{
	return cells__.size();
}

const string& Sqlite::row_t::getValue(int idx) const
{
	return getCell(idx).value;
}

const string& Sqlite::row_t::getValue(const string& col) const
{
	return getCell(col).value;
}

bool Sqlite::row_t::isNull(int idx) const
{
	return getCell(idx).isNull;
}

bool Sqlite::row_t::isNull(const string& col) const
{
	return getCell(col).isNull;
}

const string& Sqlite::row_t::getColumnName(int idx) const
{
	return getCell(idx).columnName;
}

const Sqlite::cell_t& Sqlite::row_t::getCell(int idx) const
{
	return cells__.at(idx);
}

const Sqlite::cell_t& Sqlite::row_t::getCell(const string& col) const
{
	std::map<string, int>::const_iterator it = cellsByName__.find(col);
	if (it == cellsByName__.end()) {
		throw std::out_of_range("Sqlite::row_t::getCell");
	}
	else {
		return getCell(it->second);
	}
}

Sqlite::row_t::const_iterator Sqlite::row_t::begin() const
{
	return cells__.begin();
}

Sqlite::row_t::const_iterator Sqlite::row_t::end() const
{
	return cells__.end();
}

} // end of: namespace cppmt

