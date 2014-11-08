///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2006 - 2014 by Ralf Holly.
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

/** File -- header file.
 * @file
 */

#ifndef File_h
#define File_h

#include <vector>
#include <string>

/** Encapsulates a particular issue reported against a file. */
struct FileIssue
{
    /** Constructor.
     * @param number Issue number.
     * @param line Line on which issue was found.
     */
    FileIssue(int number, int line) : m_number(number), m_line(line) { }

    /** Issue number. */
    int m_number;
    /** Line on which issue was found. */
    int m_line;
};

/** Encapsulates a source code file with possibly many lint issues. */
class File
{
public:
    /** Constructor.
     * @param filename Filename of this file.
     */
    File(const std::string& filename) :
        m_filename(filename),
        m_severityScore(0),
        m_severestIssueNumber(UNUSED_ISSUE_NUMBER) {}

    /** Registers a lint issue with this file.
     * @param number Issue number.
     * @param line Line on which issue was found.
     */
    void addIssue(int number, int line);

    /** Returns the filename of this file.
     * @return Filename of this file.
     */
    const std::string& getFilename() const {
        return m_filename;
    }

    /** Returns the severity score of this file.
     * @return Severity score.
     */
    int getSeverityScore() const {
        return m_severityScore;
    }

    /** Returns the issue number of the severest issue found in this file. 
     * @return Issue number.
     */
    int getSeverestIssueNumber() const {
        return m_severestIssueNumber;
    }

    /** Returns the total number of issues found in this file.
     * @return Total number of issues.
     */
    unsigned int getIssueCount() const {
        return m_issues.size();
    }

private:
    static const int UNUSED_ISSUE_NUMBER = 0;  // Sentinel marker value

    friend bool operator<(const File& lhs, const File& rhs);
    std::string m_filename;             // The name of this source code module
    int m_severityScore;                // The accumulated severity score
    int m_severestIssueNumber;          // The issue number with the highest severity
    std::vector<FileIssue> m_issues;    // A list of all Lint issues contained in this file
};

inline bool operator<(const File& lhs, const File&  rhs)
{
    // Sort by severity score, then filename
    if (lhs.m_severityScore == rhs.m_severityScore)
        return lhs.m_filename < rhs.m_filename;
    return lhs.m_severityScore > rhs.m_severityScore;
}

#endif

