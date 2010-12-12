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

#include "ClassicMetricsReporter.h"
#include "IssueTable.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>

using namespace std;

void ClassicMetricsReporter::reportMetrics(int totalSeverityScore, int totalIssuesCount,
        const FILE_MAP& fileMap, const ISSUE_MAP& issueMap,
        const FILE_LIST& fileList, const ISSUE_LIST& issueList)
{
    // Unused parameters.
    (void)fileMap;
    (void)issueMap;

    cout << endl
         << "Total number of issues found : " << totalIssuesCount << endl
         << "Total severity score ....... : " << totalSeverityScore << endl;
    printFileList(fileList);
    printIssueList(issueList);
    printFooter();
}

void ClassicMetricsReporter::printFileList(const FILE_LIST& fileList)
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
    for (FILE_LIST::const_iterator iter = fileList.begin();
            iter != fileList.end();
            ++iter) {
        ++pos;
        int severestIssueNr = (*iter).getSeverestIssueNumber();
        int maxSeverity = IssueTable::getSeverity(severestIssueNr);

        cout << setw(7) << pos
             << setw(7) << (*iter).getSeverityScore()
             << setw(7) << (*iter).getIssueCount()
             << setw(7) << severestIssueNr
             << setw(5) << maxSeverity
             << "   "  << (*iter).getFilename()
             << endl;
    }
}

void ClassicMetricsReporter::printIssueList(const ISSUE_LIST& issueList)
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
    for (ISSUE_LIST::const_iterator iter = issueList.begin();
            iter != issueList.end();
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

void ClassicMetricsReporter::printFooter()
{
    cout << endl
         << "Legend" << endl
         << "------" << endl
         << endl;

    for (size_t i = 0; i < IssueTable::getIssueTableSize(); ++i) {
        cout << "Severity level " << setw(3) << IssueTable::getIssueTableSeverityByIndex(i) << " : "
             << IssueTable::getIssueTableTitleByIndex(i) << endl;
    }

    cout << endl
         << "Score  Severity score" << endl
         << "MMsg   Number of issue with the highest severity encountered" << endl
         << "MSev   Severity level of the severest issue encountered" << endl
         << "Msg    Lint issue number" << endl
         << "Sev    Issue severity level" << endl
         << "Count  Total number of occurrences of issue in project" << endl
         << "Issues Total number of issues in file" << endl
         << endl;
}

