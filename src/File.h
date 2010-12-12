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

#ifndef File_h
#define File_h

#include <vector>
#include <string>

// Encapsulates a source code file with possibly many lint issues
class File
{
public:
    File(const std::string& filename) :
        m_filename(filename),
        m_severityScore(0),
        m_severestIssueNumber(UNUSED_ISSUE_NUMBER) {}

    // Registers a lint issue with this file
    void addIssue(int issueNumber);

    const std::string& getFilename() const {
        return m_filename;
    }
    int getSeverityScore() const {
        return m_severityScore;
    }
    int getSeverestIssueNumber() const {
        return m_severestIssueNumber;
    }
    unsigned int getIssueCount() const {
        return m_issues.size();
    }

private:
    static const int UNUSED_ISSUE_NUMBER = 0;  // Sentinel marker value

    friend bool operator<(const File& lhs, const File& rhs);
    std::string m_filename;         // The name of this source code module
    int m_severityScore;            // The accumulated severity score
    int m_severestIssueNumber;      // The issue number with the highest severity
    std::vector<int> m_issues;      // A list of all Lint issues contained in this file
};

inline bool operator<(const File& lhs, const File&  rhs)
{
    // Sort by severity score, then filename
    if (lhs.m_severityScore == rhs.m_severityScore)
        return lhs.m_filename < rhs.m_filename;
    return lhs.m_severityScore > rhs.m_severityScore;
}

#endif

