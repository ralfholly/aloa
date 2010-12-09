
#include "File.h"
#include <cassert>

#include "globals.h"

const int File::UNUSED_ISSUE_NUMBER; 

void File::addIssue(int issueNumber) 
{
    m_issues.push_back(issueNumber);
    int severity = ::getSeverity(issueNumber);
    if (   m_severestIssueNumber == UNUSED_ISSUE_NUMBER
            || severity > getSeverity(m_severestIssueNumber) ) {
        m_severestIssueNumber = issueNumber;
    }
    m_severityScore += severity;
}

