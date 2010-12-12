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

#ifndef Issue_h
#define Issue_h

#include <vector>
#include <string>

// Encapsulates a lint issue (ie. warning, error)
class Issue
{
public:
    Issue(int number, int severity) :
        m_number(number),
        m_severity(severity),
        m_count(0) {}

    // Register a file with this issue
    void addFile(const std::string& filename);

    int getNumber() const { 
        return m_number;
    }
    int getSeverity() const {
        return m_severity;
    }
    int getCount() const {
        return m_count;
    }

private:
    friend bool operator<(const Issue& lhs, const Issue& rhs);
    int m_number;         // Lint issue number
    int m_severity;       // The severity level of this lint issue
    int m_count;          // Total number of occurrences of this lint issue
    std::vector<std::string> m_files;  // List of all files that contain this lint issue
};

inline bool operator<(const Issue& lhs, const Issue& rhs)
{
    // Sort by count, then by severity
    if (lhs.m_count == rhs.m_count)
        return lhs.m_severity > rhs.m_severity;
    return lhs.m_count > rhs.m_count;
}

#endif

