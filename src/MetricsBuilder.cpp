#include "MetricsBuilder.h"
#include <algorithm>

#include "globals.h"
#include "IssueTable.h"
#include "MetricsReporter.h"

using namespace std;

MetricsBuilder::MetricsBuilder() :
    m_severityScore(0),
    m_issuesCount(0)
{
}

int MetricsBuilder::getSeverityScore() const
{
    return m_severityScore;
}

int MetricsBuilder::getIssuesCount() const
{
    return m_issuesCount;
}

void MetricsBuilder::addSeverityScore(int score)
{
    m_severityScore += score;
}

void MetricsBuilder::addIssuesCount(int count)
{
    m_issuesCount += count;
}

void MetricsBuilder::onNewIssue(const string& filename, int number)
{
    int severity = IssueTable::getSeverity(number);

    // Update global metrics
    addIssuesCount(1);
    addSeverityScore(severity);

    // Update metrics of this file.
    // If given filename is already known and has a corresponding File object,
    // use it; otherwise, create a new one.
    FILE_MAP::iterator iterFile = m_fileMap.find(filename);
    if (iterFile == m_fileMap.end()) {
        File file(filename);
        iterFile = m_fileMap.insert(make_pair(filename, file)).first;
    }
    File* file = &(*iterFile).second ;
    file->addIssue(number);

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

void MetricsBuilder::buildMetricsLists(MetricsReporter* reporter)
{
    // Create sorted file list
    FILE_MAP::iterator iterFile = m_fileMap.begin();
    for (; iterFile != m_fileMap.end(); ++iterFile) {
        m_fileList.push_back((*iterFile).second);
    }
    sort(m_fileList.begin(), m_fileList.end());

    // Create sorted issue list
    ISSUE_MAP::iterator iterIssue = m_issueMap.begin();
    for (; iterIssue != m_issueMap.end(); ++iterIssue) {
        m_issueList.push_back((*iterIssue).second);
    }
    sort(m_issueList.begin(), m_issueList.end());

    // Now send the metrics on to the reporter.
    assert(reporter != NULL);
    reporter->reportMetrics(
        m_severityScore,
        m_issuesCount,
        m_fileMap,
        m_issueMap,
        m_fileList,
        m_issueList);
}

