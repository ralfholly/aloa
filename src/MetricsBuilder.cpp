///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2006 - 2013 by Ralf Holly.
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

/** MetricsBuilder -- implementation file.
 * @file
 */

#include "MetricsBuilder.h"
#include "IssueTable.h"
#include "MetricsReporter.h"
#include "MisraParser.h"

#include <algorithm>
#include <cassert>

using namespace std;

MetricsBuilder::MetricsBuilder(bool misraEnabled) :
    m_severityScore(0),
    m_issuesCount(0),
    m_misraEnabled(misraEnabled)
{
}

int MetricsBuilder::getIssuesCount() const
{
    return m_issuesCount;
}

void MetricsBuilder::onNewIssue(int number, const std::string& filename, int line)
{
    int severity = IssueTable::getSeverity(number);

    // Update global metrics
    ++m_issuesCount;
    m_severityScore += severity;

    // Update metrics of this file.
    // If given filename is already known and has a corresponding File object,
    // use it; otherwise, create a new one.
    FILE_MAP::iterator iterFile = m_fileMap.find(filename);
    if (iterFile == m_fileMap.end()) {
        File file(filename);
        iterFile = m_fileMap.insert(make_pair(filename, file)).first;
    }
    File* file = &(*iterFile).second ;
    file->addIssue(number, line);

    // Update metrics of this issue.
    // If given issue is already known and has a corresponding Issue object,
    // use it; otherwise, create a new one.
    Issue* pIssue;
    ISSUE_MAP::iterator iterIssue = m_issueMap.find(number);
    if (iterIssue == m_issueMap.end()) {
        Issue issue(number, severity);
        iterIssue = m_issueMap.insert(make_pair(number, issue)).first;
    }
    pIssue = &iterIssue->second;
    pIssue->addFile(filename);
}

void MetricsBuilder::reportMetrics(const MetricsReporter* reporter, const MisraParser* misraParser)
{
    // Create sorted file list
    FILE_MAP::iterator iterFile = m_fileMap.begin();
    for (; iterFile != m_fileMap.end(); ++iterFile) {
        m_fileList.push_back((*iterFile).second);
    }
    // Note! stable_sort maintains relative order of output; this makes
    // diffing outputs easier when only few issues have changed.
    stable_sort(m_fileList.begin(), m_fileList.end());

    // Create sorted issue list
    ISSUE_MAP::iterator iterIssue = m_issueMap.begin();
    for (; iterIssue != m_issueMap.end(); ++iterIssue) {
        m_issueList.push_back((*iterIssue).second);
    }
    // Note! stable_sort maintains relative order of output; this makes
    // diffing outputs easier when only few issues have changed.
    stable_sort(m_issueList.begin(), m_issueList.end());

    // Now send the metrics on to the reporter.
    assert(reporter != NULL);
    reporter->reportMetrics(
        m_severityScore,
        m_issuesCount,
        m_fileMap,
        m_issueMap,
        m_fileList,
        m_issueList,
        misraParser != NULL ? misraParser->getMisraStringIssueList() : NULL);
}

