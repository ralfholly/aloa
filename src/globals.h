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

const std::string VERSION   = "4.1.0"; 
const std::string YEAR      = "2010";
const std::string COPYRIGHT = "ALOA version " + VERSION + 
    ". Copyright " + YEAR + " by Ralf Holly.\n";
const std::string COPYLEFT  =
    "This program is free software according to the GNU\n"
    "General Public License and comes WITHOUT ANY WARRANTY.\n"
    "For details, run aloa without arguments.\n";

const int MAX_ISSUE_NUMBER         = 2000;    // Highest possible lint issue number
const int UNUSED_ISSUE_NUMBER      = 0;       // Sentinel marker value
void initGlobals(void);
int getSeverity(int number);

class File;

// Encapsulates a type of Lint issue (ie. error, warning, informational)
//lint --e{1712} Default ctor is fine
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

// Encapsulates a lint issue (ie. warning, error)
//lint --e{1712} Default ctor is fine
class Issue
{
public:
    Issue(int number, int severity) :
        m_number(number),
        m_severity(severity),
        m_count(0) {}

    // Register a file with this issue
    void addFile(const File* pFile) {
        assert(pFile != NULL);
        m_files.push_back(pFile);
        ++m_count;
    }

    int getNumber() const       {
        return m_number;
    }
    int getSeverity() const     {
        return m_severity;
    }
    int getCount() const        {
        return m_count;
    }

private:
    friend bool operator<(const Issue& lhs, const Issue& rhs);
    typedef std::vector<const File*> FileList;
    int m_number;         // Lint issue number
    int m_severity;       // The severity level of this lint issue
    int m_count;          // Total number of occurrences of this lint issue
    FileList m_files;     // List of all files that contain this lint issue
};

inline bool operator<(const Issue& lhs, const Issue& rhs)
{
    // Sort by count, then by severity
    if (lhs.m_count == rhs.m_count)
        return lhs.m_severity > rhs.m_severity;
    return lhs.m_count > rhs.m_count;
}


// Encapsulates a source code file with possibly many lint issues
//lint --e{1712} Default ctor is fine
class File
{
public:
    File(const std::string& filename) :
        m_filename(filename),
        m_severityScore(0),
        m_severestIssueNumber(UNUSED_ISSUE_NUMBER) {}

    // Registers a lint issue with this file
    void addIssue(const Issue* pIssue) {
        assert(pIssue != NULL);
        m_issues.push_back(pIssue);
        int issueNumber = pIssue->getNumber();
        int severity = getSeverity(issueNumber);
        if (   m_severestIssueNumber == UNUSED_ISSUE_NUMBER
                || severity > getSeverity(m_severestIssueNumber) ) {
            m_severestIssueNumber = issueNumber;
        }
        m_severityScore += severity;
    }

    const std::string& getFilename() const   {
        return m_filename;
    }
    int getSeverityScore() const             {
        return m_severityScore;
    }
    int getSeverestIssueNumber() const       {
        return m_severestIssueNumber;
    }
    unsigned int getIssueCount() const       {
        return m_issues.size();
    }

private:
    friend bool operator<(const File& lhs, const File& rhs);
    typedef std::vector<const Issue*> IssueList;
    std::string m_filename;         // The name of this source code module
    int m_severityScore;            // The accumulated severity score
    int m_severestIssueNumber;      // The issue number with the highest severity
    IssueList m_issues;             // A list of all Lint issues contained in this file
};

inline bool operator<(const File& lhs, const File&  rhs)
{
    // Sort by severity score, then filename
    if (lhs.m_severityScore == rhs.m_severityScore)
        return lhs.m_filename < rhs.m_filename;
    return lhs.m_severityScore > rhs.m_severityScore;
}

typedef std::vector<IssueType> ISSUETYPE_LIST;
typedef std::map<std::string, File*> FILE_MAP;
typedef std::map<int, Issue*> ISSUE_MAP;
typedef std::vector<File> FILE_LIST;
typedef std::vector<Issue> ISSUE_LIST;

extern ISSUETYPE_LIST gIssueTypeList;
extern const char* gpLintOutputFile;
extern int gSeverityScore;
extern int gIssuesCount;
extern FILE_MAP gFileMap;
extern ISSUE_MAP gIssueMap;
extern FILE_LIST gFileList;
extern ISSUE_LIST gIssueList;


#endif // _globals_h

