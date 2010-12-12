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

#include "File.h"
#include "IssueTable.h"

#include <cassert>

const int File::UNUSED_ISSUE_NUMBER; 

void File::addIssue(int issueNumber, int line) 
{
    m_issues.push_back(FileIssue(issueNumber, line));
    int severity = IssueTable::getSeverity(issueNumber);
    if (   m_severestIssueNumber == UNUSED_ISSUE_NUMBER
            || severity > IssueTable::getSeverity(m_severestIssueNumber) ) {
        m_severestIssueNumber = issueNumber;
    }
    m_severityScore += severity;
}

