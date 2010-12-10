
#include "File.h"
#include <cassert>

#include "globals.h"
#include "IssueTable.h"

const int File::UNUSED_ISSUE_NUMBER; 

void File::addIssue(int issueNumber) 
{
    m_issues.push_back(issueNumber);
    int severity = IssueTable::getSeverity(issueNumber);
    if (   m_severestIssueNumber == UNUSED_ISSUE_NUMBER
            || severity > IssueTable::getSeverity(m_severestIssueNumber) ) {
        m_severestIssueNumber = issueNumber;
    }
    m_severityScore += severity;
}

