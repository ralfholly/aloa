///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 by Ralf Holly.
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

#ifndef IssueTable_h
#define IssueTable_h

#include <cstddef>

struct IssueTableEntry
{
    const char* title;
    int severity;
    int lowerBound;
    int upperBound;
};

const IssueTableEntry ISSUE_TABLE[] = 
{
    { "Elective note",       1, 900, 999 },
    { "Informational",       2, 700, 899 },
    { "Warning",             3, 400, 699 },
    { "Syntax error",        4,   1, 199 },
    { "PC-Lint error",     999, 200, 399 },
};

class IssueTable
{
public:
    static const int MAX_ISSUE_NUMBER = 2000;
    static size_t getIssueTableSize();
    static int getSeverity(int number);
    static int getIssueTableSeverityByIndex(size_t i);
    static const char* getIssueTableTitleByIndex(size_t i);
};

#endif

