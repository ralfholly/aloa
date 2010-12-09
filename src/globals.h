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

#ifndef _globals_h
#define _globals_h

#include <string>
#include <vector>
#include <map>
#include <assert.h>

#include "File.h"
#include "Issue.h"
typedef std::vector<File> FILE_LIST;
typedef std::vector<Issue> ISSUE_LIST;

const std::string VERSION   = "4.1.0"; 
const std::string YEAR      = "2010";
const std::string COPYRIGHT = "ALOA version " + VERSION + 
    ". Copyright " + YEAR + " by Ralf Holly.\n";
const std::string COPYLEFT  =
    "This program is free software according to the GNU\n"
    "General Public License and comes WITHOUT ANY WARRANTY.\n"
    "For details, run aloa without arguments.\n";

const int MAX_ISSUE_NUMBER         = 2000;    // Highest possible lint issue number
void initGlobals(void);
int getSeverity(int number);


// Encapsulates a type of Lint issue (ie. error, warning, informational)
class IssueType
{
public:
    IssueType(const std::string& issueName, int severity, int lowerBound, int upperBound) :
        m_issueName(issueName), m_severity(severity),
        m_lowerBound(lowerBound), m_upperBound(upperBound) {}
    const std::string& getIssueName() const      {
        return m_issueName;
    }
    int getSeverity() const                      {
        return m_severity;
    }
    bool isThisIssueType(int issueNo) const      {
        return m_lowerBound <= issueNo && issueNo <= m_upperBound;
    }

private:
    std::string m_issueName;        // Descriptive name of this issue type
    int m_severity;                 // Severity level of this issue type
    int m_lowerBound;               // Issue numbers in the range [m_lowerBound;m_upperBound]
    int m_upperBound;               // belong to this issue type
};


typedef std::vector<IssueType> ISSUETYPE_LIST;

extern ISSUETYPE_LIST gIssueTypeList;
extern const char* gpLintOutputFile;
extern int gSeverityScore;
extern int gIssuesCount;
extern FILE_LIST gFileList;
extern ISSUE_LIST gIssueList;


#endif // _globals_h

