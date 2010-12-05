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

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>

#include "globals.h"
#include "report.h"

using namespace std;

static void printFileList()
{
    cout << endl
         << "File List" << endl
         << "-----------" << endl
         << endl
         << setw(7) << "Rank"
         << setw(7) << "Score"
         << setw(7) << "Issues"
         << setw(7) << "MMsg"
         << setw(5) << "MSev"
         << "   " << "File"
         << endl
         << endl;

    int pos = 0;
    for (FILE_LIST::iterator iter = gFileList.begin();
            iter != gFileList.end();
            ++iter) {
        ++pos;
        int severestIssueNr = (*iter).getSeverestIssueNumber();
        int maxSeverity = getSeverity(severestIssueNr);

        cout << setw(7) << pos
             << setw(7) << (*iter).getSeverityScore()
             << setw(7) << (*iter).getIssueCount()
             << setw(7) << severestIssueNr
             << setw(5) << maxSeverity
             << "   "  << (*iter).getFilename()
             << endl;
    }
}

static void printIssueList()
{

    cout << endl
         << "Issue List" << endl
         << "----------" << endl
         << endl
         << setw(7) << "Rank"
         << setw(7) << "Msg"
         << setw(5) << "Sev"
         << setw(7) << "Count"
         << endl
         << endl;

    int pos = 0;
    for (ISSUE_LIST::iterator iter = gIssueList.begin();
            iter != gIssueList.end();
            ++iter) {
        ++pos;
        int severity = (*iter).getSeverity();
        cout << setw(7) << pos
             << setw(7) << (*iter).getNumber()
             << setw(5) << severity
             << setw(7) << (*iter).getCount()
             << endl;
    }
}

static void printFooter()
{
    cout << endl
         << "Legend" << endl
         << "------" << endl
         << endl;

    ISSUETYPE_LIST::iterator iter = gIssueTypeList.begin();
    for (; iter != gIssueTypeList.end(); ++iter) {
        cout << "Severity level " << setw(3) << (*iter).getSeverity() << " : "
             << (*iter).getIssueName() << endl;
    }

    cout << endl
         << "Score  Severity score" << endl
         << "MMsg   Number of issue with the highest severity encountered" << endl
         << "MSev   Severity level of the severest issue encountered" << endl
         << "Msg    Lint issue number" << endl
         << "Sev    Issue severity level" << endl
         << "Count  Total number of occurrences of issue in project" << endl
         << "Issues Total number of issues in file" << endl
         << endl
         << endl;
}

void reportMetrics()
{
    cout << endl
         << COPYRIGHT
         << COPYLEFT
         << endl
         << "Total number of issues found : " << gIssuesCount << endl
         << "Total severity score ....... : " << gSeverityScore << endl;

    printFileList();
    printIssueList();
    printFooter();
}


void reportIllegalLintIssueNumber(void)
{
    ostringstream s;
    s << "Lint message numbers below 1 and above "
      << MAX_ISSUE_NUMBER << " are not supported" << endl;
    reportFatalError(s.str());
}

void reportFatalError(const string& message)
{
    cerr << endl << "Fatal error: " << message << endl;
    exit(EXIT_FAILURE);
}
