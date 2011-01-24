///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2006 - 2010 by Ralf Holly.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
///////////////////////////////////////////////////////////////////////////////

#include "IssueTable.h"

#include <cassert>

const IssueTable::IssueTableEntry IssueTable::ISSUE_TABLE[] = 
{
//    Category             Sev  From  To
//    -----------------------------------
    { "Elective note",       1, 900, 999 },
    { "Informational",       2, 700, 899 },
    { "Warning",             3, 400, 699 },
    { "Syntax error",        4,   1, 199 },
    { "PC-Lint error",     999, 200, 399 },
};

size_t IssueTable::getIssueTableSize()
{
    return sizeof(ISSUE_TABLE) / sizeof(ISSUE_TABLE[0]);
}

int IssueTable::getSeverity(int number)
{
    // Issue numbers 1xxx have same severity as numbers xxx.
    number %= 1000;

    const size_t tableSize = getIssueTableSize();

    for (size_t i = 0; i < tableSize; ++i) {
        if (ISSUE_TABLE[i].lowerBound <= number &&
                ISSUE_TABLE[i].upperBound >= number) {
            return ISSUE_TABLE[i].severity;
        }
    }
    return -1;
}

const char* IssueTable::getIssueTableTitleByIndex(size_t i)
{
    const size_t tableSize = getIssueTableSize();
    assert(i < tableSize);
    return ISSUE_TABLE[i].title;
}

int IssueTable::getIssueTableSeverityByIndex(size_t i)
{
    const size_t tableSize = getIssueTableSize();
    assert(i < tableSize);
    return ISSUE_TABLE[i].severity;
}

