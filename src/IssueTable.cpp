///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2006 - 2014 by Ralf Holly.
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
//    Category             Sev     From      To
//    -------------------------------------------------
    { "C Syntax Errors",     4,       1,     199    },
    { "Internal Errors",   999,     200,     299    },
    { "Fatal Errors",      999,     300,     399    },
    { "C Warnings",          3,     400,     699    },
    { "C Informational",     2,     700,     899    },
    { "C Elective Notes",    1,     900,     999    },
    { "C++ Syntax Errors",   4,    1000,    1199    },
    { "Internal Errors",   999,    1200,    1299    },
    { "C++ Fatal Erros",   999,    1300,    1399    },
    { "C++ Warnings",        3,    1400,    1699    },
    { "C++ Informational",   2,    1700,    1899    },
    { "C++ Elective Notes",  1,    1900,    1999    },
    { "C Syntax Errors",     4,    2000,    2199    },
    { "Reserved",          999,    2200,    2399    },
    { "C Warnings",          3,    2400,    2699    },
    { "C Informational",     2,    2700,    2899    },
    { "C Elective Notes",    1,    2900,    2999    },
    { "C++ Syntax Errors",   4,    3000,    3199    },
    { "Reserved",          999,    3200,    3399    },
    { "C++ Warnings",        3,    3400,    3699    },
    { "C++ Informational",   2,    3900,    3999    },
    { "C++ Elective Notes",  1,    3900,    3999    },
    { "C and C++ Errors",    4,    4000,    5999    },
    { "Reserved",          999,    6000,    6999    },
    { "Reserved",          999,    7000,    7999    },
    { "User Defined",        2,    8000,    8999    },
    { "Misc Elective Notes", 1,    9000,    9999    },
};

size_t IssueTable::getIssueTableSize()
{
    return sizeof(ISSUE_TABLE) / sizeof(ISSUE_TABLE[0]);
}

int IssueTable::getSeverity(int number)
{
    // If virtual issue number.
    if (VIRTUAL_ISSUE_LOWER_BOUND <= number && number <= VIRTUAL_ISSUE_UPPER_BOUND) {
        return VIRTUAL_ISSUE_SEVERITY;
    }
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

