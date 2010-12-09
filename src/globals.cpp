///////////////////////////////////////////////////////////////////////////////
// ALOA - A Lint Output Analyzer
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

#include <string>
#include "globals.h"

using namespace std;

ISSUETYPE_LIST gIssueTypeList;
FILE_LIST gFileList;
ISSUE_LIST gIssueList;

void initGlobals(void)
{
    // Register types of issues and their severity level
    gIssueTypeList.push_back(IssueType("Elective note",       1, 900, 999));
    gIssueTypeList.push_back(IssueType("Informational",       2, 700, 899));
    gIssueTypeList.push_back(IssueType("Warning",             3, 400, 699));
    gIssueTypeList.push_back(IssueType("Syntax error",        4,   1, 199));
    gIssueTypeList.push_back(IssueType("PC-Lint error",     999, 200, 399));
}

// Determines the severity level of a lint issue
int getSeverity(int number)
{
    extern void reportIllegalLintIssueNumber(void);
    // C++ issue numbers have an offset of 1000 compared to C issue numbers.
    // However, the severity levels are identical
    if (number >= 1000)
        number -= 1000;

    ISSUETYPE_LIST::iterator iter = gIssueTypeList.begin();
    for (; iter != gIssueTypeList.end(); ++iter) {
        if ((*iter).isThisIssueType(number)) {
            return (*iter).getSeverity();
        }
    }

    reportIllegalLintIssueNumber();
    return -1; // dummy, unreachable
}




